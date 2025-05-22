#include <cstdint>
#include <cstddef>
#include <filesystem>
#include <unordered_map>
#include <windows.h>
#include <ShlObj.h>
#include "file_system.hpp"
#include "../../sunset/sunset.hpp"
#include "../../logger.hpp"
#include "../../localization.hpp"
#include "../../config.hpp"
#include "../../util/mutex.hpp"

auto tvg_operator_new = (void* (_cdecl*)(std::size_t))(0x0068b173);
auto tvg_operator_delete = (void(_cdecl*)(void*))(0x0068ae53);
auto tvg_fopen = (FILE* (_cdecl*)(const char*, const char*))(0x0068ef6a);
auto tvg_fclose = (int(_cdecl*)(FILE*))(0x0068ed88);
auto tvg_BinkOpen = (struct BINK* (__stdcall*)(const char*, std::uint32_t))(nullptr);

#pragma pack(push, 1) // Ensure packing alignment

struct FileInfo {
    char path[100];
    std::uint32_t offset;
    std::uint32_t size;
};

struct LoadedPak {
    char path[100];
    FileInfo* files_in_pak;
    std::size_t file_count;
    std::uint32_t file_data_start_offset;
};

struct LoadedFile {
    FILE* pak_file_pointer;
    FileInfo* file_info;
    const LoadedPak* loaded_pak;
    std::uint32_t seek;
};

struct PakSystem {
    LoadedPak* loaded_paks;
    std::uint32_t pak_count;
    std::uint32_t unk;
    RTL_CRITICAL_SECTION lock;
};

#pragma pack(pop) // Restore packing alignment

// Used to ensure that the game can access a LoadedPak's file_data_start_offset from a LoadedFile.
// (The game does this at 0x00687f20 in the Steam version of the game)
static LoadedPak DUMMY_LOADED_PAK = LoadedPak{
    .path = {},
    .files_in_pak = 0,
    .file_count = 0,
    .file_data_start_offset = 0,
};

struct LoadedFilesystem {
    std::filesystem::path mods_directory;
    std::unordered_map<std::string, std::size_t> file_path_to_index;
    std::vector<std::string> mod_names;

    auto collect_files(const std::vector<std::string>& mods_enabled) -> bool;
    auto lookup(std::string relative_path) const->std::optional<std::pair<std::filesystem::path, std::string>>;
    auto save_dir() const->std::filesystem::path;
};

util::Mutex<LoadedFilesystem> LOADED_FS{};

auto LoadedFilesystem::collect_files(const std::vector<std::string>& mods_enabled) -> bool {
    for (const auto& mod_name : mods_enabled) {
        std::filesystem::path mod_folder = mods_directory / mod_name;
        if (std::filesystem::is_directory(mod_folder)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(mod_folder)) {
                if (entry.is_regular_file()) {
                    std::wstring fname = entry.path().filename().native();
                    // Get the `DataPC`-relative file path, and force it to be lowercase.
                    std::string relative_file_path = entry.path().lexically_relative(mod_folder).string();
                    std::transform(relative_file_path.begin(), relative_file_path.end(), relative_file_path.begin(), [](std::uint8_t c) { return std::tolower(c); });
                    if (file_path_to_index.contains(relative_file_path)) {
                        LOG_LOCALIZED_STRING(FILE_IN_MOD_OVERWRITES_EXISTING, relative_file_path, mod_name, mod_names[file_path_to_index.at(relative_file_path)]);
                    }
                    else {
                        LOG_LOCALIZED_STRING(COLLECTED_FILE, relative_file_path, mod_name);
                    }
                    file_path_to_index.insert_or_assign(relative_file_path, mod_names.size());
                }
            }
            mod_names.push_back(mod_name);
        }
        else {
            LOG_LOCALIZED_STRING(MOD_REJECTED_DOES_NOT_EXIST, mod_name);
        }
    }
    return true;
}

auto LoadedFilesystem::lookup(std::string relative_path) const -> std::optional<std::pair<std::filesystem::path, std::string>> {
    std::transform(relative_path.begin(), relative_path.end(), relative_path.begin(), [](std::uint8_t c) { return std::tolower(c); });
    if (file_path_to_index.contains(relative_path)) {
        const std::string& mod_name = mod_names[file_path_to_index.at(relative_path)];
        return std::make_pair(mods_directory / mod_name / relative_path, mod_name);
    }
    return std::nullopt;
}

auto LoadedFilesystem::save_dir() const -> std::filesystem::path {
    wchar_t documents[1024];
    SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, documents);
    return std::filesystem::path(documents) / L"\\THQ\\Cars\\Save\\";
}

struct BINK* __stdcall BinkOpenHook(char* file, std::uint32_t flags) {
    const auto guard = LOADED_FS.lock();
    std::string relative_path = file;
    relative_path = relative_path.substr(sizeof("Data\\") - 1);
    if (auto patch_file = guard->lookup(relative_path); patch_file.has_value()) {
        const std::filesystem::path& output_file = patch_file.value().first;
        const std::string& mod_name = patch_file.value().second;
        std::string output_file_path = patch_file.value().first.string();
        LOG_LOCALIZED_STRING(BINK_LOADING_MODDED_FILE, relative_path, mod_name);
        return tvg_BinkOpen(output_file_path.c_str(), flags);
    }
    return tvg_BinkOpen(file, flags);
}

// Stubs the function responsible for mounting Pak files so they are no longer necessary.
DefineReplacementHook(MountPakFile) {
    static void __fastcall callback(PakSystem* _this, std::uintptr_t edx, const char* path) {
        return;
    }
};

// Hooks the function responsible for freeing LoadedFiles so our FileInfos get properly free'd.
DefineReplacementHook(FreeLoadedFile) {
    static int _fastcall callback(PakSystem* _this, std::uintptr_t edx, LoadedFile* file) {
        EnterCriticalSection(&_this->lock);
        tvg_fclose(file->pak_file_pointer);
        tvg_operator_delete(file->file_info);
        tvg_operator_delete(file);
        LeaveCriticalSection(&_this->lock);
        return 0;
    }
};

DefineReplacementHook(PakOpenFile) {
    static LoadedFile* __fastcall callback(PakSystem * _this, std::uintptr_t edx, const char* path, std::uint32_t _unk) {
        const auto guard = LOADED_FS.lock();
        auto* file = reinterpret_cast<LoadedFile*>(tvg_operator_new(sizeof(LoadedFile)));
        file->pak_file_pointer = nullptr;
        file->file_info = nullptr;
        file->loaded_pak = nullptr;
        file->seek = 0;

        std::string relative_path = path;
        relative_path = relative_path.substr(sizeof("Data\\") - 1);

        // If we have a mod file for the given path, try to load it.
        if (auto patch_file = guard->lookup(relative_path); patch_file.has_value()) {
            const std::filesystem::path& output_file = patch_file.value().first;
            const std::string& mod_name = patch_file.value().second;
            std::string output_file_path = patch_file.value().first.string();
            LOG_LOCALIZED_STRING(TVG_PAK_LOADING_MODDED_FILE, relative_path, mod_name);
            file->pak_file_pointer = tvg_fopen(output_file_path.data(), "rb");

            // Here, we allocate a new dummy FileInfo to ensure that the game can access its "offset" which we set to zero as, of course, modded files not inside a Pak don't need one.
            // We do, however, give it an actual proper file size. This is why we need to allocate it ourselves, since it must be unique.
            // (The game accesses this at 0x00687f20 in the Steam version of the game)
            auto* file_info = reinterpret_cast<FileInfo*>(tvg_operator_new(sizeof(FileInfo)));
            std::memset(file_info->path, 0, sizeof(file_info->path));
            file_info->offset = 0;
            file_info->size = static_cast<std::uint32_t>(std::filesystem::file_size(output_file));

            file->file_info = file_info;
            file->loaded_pak = &DUMMY_LOADED_PAK;
            return file;
        }
        // Otherwise, fall back to loading from the `Data` directory.
        else if (std::filesystem::is_regular_file(path)) {
            LOG_LOCALIZED_STRING(TVG_PAK_LOADING_BASE_FILE, relative_path);
            file->pak_file_pointer = tvg_fopen(path, "rb");

            // Here, we allocate a new dummy FileInfo to ensure that the game can access its "offset" which we set to zero as, of course, modded files not inside a Pak don't need one.
            // We do, however, give it an actual proper file size. This is why we need to allocate it ourselves, since it must be unique.
            // (The game accesses this at 0x00687f20 in the Steam version of the game)
            auto* file_info = reinterpret_cast<FileInfo*>(tvg_operator_new(sizeof(FileInfo)));
            std::memset(file_info->path, 0, sizeof(file_info->path));
            file_info->offset = 0;
            file_info->size = static_cast<std::uint32_t>(std::filesystem::file_size(path));

            file->file_info = file_info;
            file->loaded_pak = &DUMMY_LOADED_PAK;
            return file;
        }
        // If all else fails, free the `file` and return nullptr.
        else {
            tvg_operator_delete(file);
            return nullptr;
        }
    }
};

auto tvg::fs::init(const std::filesystem::path& install_directory,
    const std::filesystem::path& base_data_directory,
    const std::filesystem::path& mods_directory,
    const std::vector<std::string>& mods_enabled) -> bool
{
    tvg_BinkOpen = reinterpret_cast<struct BINK* (__stdcall*)(const char*, std::uint32_t)>(GetProcAddress(GetModuleHandleA("binkw32.dll"), "_BinkOpen@8"));
    if (tvg_BinkOpen == nullptr) {
        LOG_LOCALIZED_STRING(MN_FS_FAILED_INIT);
    }

    auto guard = LOADED_FS.lock_mut();
    guard->mods_directory = mods_directory;

	std::string base_data_directory_name = base_data_directory.lexically_relative(base_data_directory.parent_path()).string();

	if (!mods_enabled.empty()) {
        guard->collect_files(mods_enabled);
	}

    MountPakFile::install_at_ptr(0x00687a60);
    FreeLoadedFile::install_at_ptr(0x00687ef0);
    PakOpenFile::install_at_ptr(0x00687d90);

    sunset::utils::set_permission(reinterpret_cast<void*>(0x007112c4), 4, sunset::utils::Perm::ReadWrite);
    *reinterpret_cast<void**>(0x007112c4) = BinkOpenHook;
    return true;
}