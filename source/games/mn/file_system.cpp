#include <filesystem>
#include <mutex>
#include <cwctype>
#include <unordered_map>
#include <unordered_set>

#include <Shlobj.h>

#include "file_system.hpp"
#include "../../sunset/sunset.hpp"
#include "../../config.hpp"
#include "../../logger.hpp"
#include "../../localization.hpp"

auto __fsopen = (void* (__cdecl*)(const char*, const char*, int))(0x0063FBFB);
auto BASS_SampleLoad = (std::uint32_t(__stdcall*)(std::int32_t, const char*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t))(nullptr);
auto BASS_StreamCreateFile = (std::uint32_t(__stdcall*)(std::int32_t, const char*, std::uint32_t, std::uint32_t, std::uint32_t))(nullptr);
auto BinkOpen = (struct BINK* (__stdcall*)(const char*, std::uint32_t))(nullptr);

struct LoadedFilesystem {
	bool save_file_redirection_enabled;
	std::filesystem::path base_data_directory;
	std::filesystem::path save_redirection_directory;
	std::filesystem::path mods_directory;
	std::unordered_map<std::string, std::size_t> file_path_to_index;
	std::vector<std::string> mod_names;

	auto collect_files(const std::vector<std::string>& mods_enabled) -> bool;
	auto data_relative_path(const std::filesystem::path& base, const std::filesystem::path& absolute_path) const -> std::string;
	auto data_relative_path(const std::filesystem::path& absolute_path) const->std::string;
	auto is_data_file(const std::filesystem::path& absolute_path) const -> bool;
	auto lookup(const std::string& relative_path) const -> std::optional<std::pair<std::filesystem::path, std::string>>;
	auto save_dir() const -> std::filesystem::path;
};

// config.ini and language.ini are forbidden from being modified via the filesystem.
std::unordered_set<std::wstring> MOD_FILES_BLACKLIST{ L"config.ini", L"language.ini" };
LoadedFilesystem LOADED_FS{};
std::mutex LOADED_FS_LOCK{};
static char BASE_DATA_DIRECTORY_NAME_C_FMT[1024] = "%s\\DataPC\\";

auto LoadedFilesystem::collect_files(const std::vector<std::string>& mods_enabled) -> bool {
	for (const auto& mod_name : mods_enabled) {
		std::filesystem::path mod_folder = mods_directory / mod_name;
		if (std::filesystem::is_directory(mod_folder)) {
			for (const auto& entry : std::filesystem::recursive_directory_iterator(mod_folder)) {
				std::wstring fname = entry.path().filename().native();
				if (!MOD_FILES_BLACKLIST.contains(fname)) {
					// Get the `DataPC`-relative file path, and force it to be lowercase.
					std::string relative_file_path = data_relative_path(mods_directory / mod_name, entry.path());
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

auto LoadedFilesystem::data_relative_path(const std::filesystem::path& base, const std::filesystem::path& absolute_path) const -> std::string {
	std::string relative_file_path = absolute_path.lexically_relative(base).string();
	std::transform(relative_file_path.begin(), relative_file_path.end(), relative_file_path.begin(), [](std::uint8_t c) { return std::tolower(c); });
	return relative_file_path;
}

auto LoadedFilesystem::data_relative_path(const std::filesystem::path& absolute_path) const -> std::string {
	std::string relative_file_path = absolute_path.lexically_relative(base_data_directory).string();
	std::transform(relative_file_path.begin(), relative_file_path.end(), relative_file_path.begin(), [](std::uint8_t c) { return std::tolower(c); });
	return relative_file_path;
}

auto LoadedFilesystem::is_data_file(const std::filesystem::path& absolute_path) const -> bool {
	auto base_iter = base_data_directory.begin();
	auto path_iter = absolute_path.begin();

	while (base_iter != base_data_directory.end() && path_iter != absolute_path.end()) {
		if (*base_iter != *path_iter) {
			return false;
		}
		++base_iter;
		++path_iter;
	}

	// If we've successfully matched the entire base_data_directory, it's a data file
	return base_iter == base_data_directory.end();
}

auto LoadedFilesystem::lookup(const std::string& relative_path) const -> std::optional<std::pair<std::filesystem::path, std::string>> {
	if (file_path_to_index.contains(relative_path)) {
		const std::string& mod_name = mod_names[file_path_to_index.at(relative_path)];
		return std::make_pair(mods_directory / mod_name / relative_path, mod_name);
	}
	return std::nullopt;
}

auto LoadedFilesystem::save_dir() const -> std::filesystem::path {
	return save_redirection_directory;
}

DefineReplacementHook(fopenHook) {
	static void* __cdecl callback(char* _Filename, char* _Mode) {
		std::scoped_lock<std::mutex> lock(LOADED_FS_LOCK);
		std::filesystem::path absolute_file_path = _Filename;
		if (LOADED_FS.is_data_file(absolute_file_path)) {
			// Get the `DataPC`-relative path.
			std::string relative_path = LOADED_FS.data_relative_path(absolute_file_path);
			if (auto patch_file = LOADED_FS.lookup(relative_path); patch_file.has_value()) {
				const std::filesystem::path& output_file = patch_file.value().first;
				const std::string& mod_name = patch_file.value().second;
				std::string output_file_path = patch_file.value().first.string();
				LOG_LOCALIZED_STRING(FOPEN_LOADING_FILE, relative_path, mod_name);
				return __fsopen(output_file_path.c_str(), _Mode, _SH_DENYNO);
			}
		}
		return __fsopen(_Filename, _Mode, 64);
	}
};

struct BINK* __stdcall BinkOpenHook(char* file, std::uint32_t flags) {
	std::scoped_lock<std::mutex> lock(LOADED_FS_LOCK);
	std::filesystem::path absolute_file_path = file;
	if (LOADED_FS.is_data_file(absolute_file_path)) {
		// Get the `DataPC`-relative path.
		std::string relative_path = LOADED_FS.data_relative_path(absolute_file_path);
		if (auto patch_file = LOADED_FS.lookup(relative_path); patch_file.has_value()) {
			const std::filesystem::path& output_file = patch_file.value().first;
			const std::string& mod_name = patch_file.value().second;
			std::string output_file_path = patch_file.value().first.string();
			LOG_LOCALIZED_STRING(MN_BINK_LOADING_FILE, relative_path, mod_name);
			return BinkOpen(output_file_path.c_str(), flags);
		}
	}
	return BinkOpen(file, flags);
}

std::uint32_t __stdcall BASS_StreamCreateFileHook(std::int32_t mem, char* file, std::uint32_t offset, std::uint32_t length, std::uint32_t flags) {
	std::scoped_lock<std::mutex> lock(LOADED_FS_LOCK);
	std::filesystem::path absolute_file_path = file;
	if (LOADED_FS.is_data_file(absolute_file_path)) {
		// Get the `DataPC`-relative path.
		std::string relative_path = LOADED_FS.data_relative_path(absolute_file_path);
		if (auto patch_file = LOADED_FS.lookup(relative_path); patch_file.has_value()) {
			const std::filesystem::path& output_file = patch_file.value().first;
			const std::string& mod_name = patch_file.value().second;
			std::string output_file_path = patch_file.value().first.string();
			LOG_LOCALIZED_STRING(STREAM_LOADING_FILE, relative_path, mod_name);
			return BASS_StreamCreateFile(mem, output_file_path.c_str(), offset, length, flags);
		}
	}
	return BASS_StreamCreateFile(mem, file, offset, length, flags);
}

std::uint32_t __stdcall BASS_SampleLoadHook(std::int32_t mem, char* file, std::uint32_t offset, std::uint32_t length, std::uint32_t max, std::uint32_t flags) {
	std::scoped_lock<std::mutex> lock(LOADED_FS_LOCK);
	std::filesystem::path absolute_file_path = file;
	if (LOADED_FS.is_data_file(absolute_file_path)) {
		// Get the `DataPC`-relative path.
		std::string relative_path = LOADED_FS.data_relative_path(absolute_file_path);
		if (auto patch_file = LOADED_FS.lookup(relative_path); patch_file.has_value()) {
			const std::filesystem::path& output_file = patch_file.value().first;
			const std::string& mod_name = patch_file.value().second;
			std::string output_file_path = patch_file.value().first.string();
			LOG_LOCALIZED_STRING(SAMPLE_LOADING_FILE, relative_path, mod_name);
			return BASS_SampleLoad(mem, output_file_path.c_str(), offset, length, max, flags);
		}
	}
	return BASS_SampleLoad(mem, file, offset, length, max, flags);
}

DefineReplacementHook(TRCEngineGetSaveDir) {
	static void __fastcall callback(char* _this) {
		std::scoped_lock<std::mutex> lock(LOADED_FS_LOCK);
		std::filesystem::path dir = LOADED_FS.save_dir();
		if (!std::filesystem::is_directory(dir)) {
			std::filesystem::create_directories(dir);
		}
		std::string str = dir.string();
		std::memcpy(_this, str.data(), str.size());
		_this[str.size()] = 0;
	}
};

// After fs::init, every access LOADED_FS must be protected via LOADED_FS_LOCK.
auto mn::fs::init(bool enable_save_redirection,
	const std::filesystem::path& install_directory,
	const std::filesystem::path& save_redirection_directory,
	const std::filesystem::path& base_data_directory,
	const std::filesystem::path& mods_directory,
	const std::vector<std::string>& mods_enabled) -> bool
{
	BinkOpen = reinterpret_cast<struct BINK* (__stdcall*)(const char*, std::uint32_t)>(GetProcAddress(GetModuleHandleA("binkw32.dll"), "_BinkOpen@8"));
	BASS_SampleLoad = reinterpret_cast<std::uint32_t(__stdcall*)(std::int32_t, const char*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t)>(GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"));
	BASS_StreamCreateFile = reinterpret_cast<std::uint32_t(__stdcall*)(std::int32_t, const char*, std::uint32_t, std::uint32_t, std::uint32_t)>(GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"));

	if (BinkOpen == nullptr || BASS_SampleLoad == nullptr || BASS_StreamCreateFile == nullptr) {
		LOG_LOCALIZED_STRING(FS_FAILED_INIT);
	}

	LOADED_FS.base_data_directory = base_data_directory;
	LOADED_FS.save_file_redirection_enabled = enable_save_redirection && save_redirection_directory.string().length() < 256;
	LOADED_FS.mods_directory = mods_directory;

	std::string base_data_directory_name = base_data_directory.lexically_relative(base_data_directory.parent_path()).string();
	std::string base_data_directory_name_format = std::format("%s\\{}\\", base_data_directory_name);
	if (base_data_directory_name_format.size() >= sizeof(BASE_DATA_DIRECTORY_NAME_C_FMT)) {
		LOADED_FS.base_data_directory = install_directory / L"DataPC";
		LOG_LOCALIZED_STRING(DATA_DIR_TOO_LARGE, base_data_directory_name);
	}

	// Update the data directory string format used by MN's WinMain.
	std::memcpy(BASE_DATA_DIRECTORY_NAME_C_FMT, base_data_directory_name_format.data(), base_data_directory_name_format.size());
	sunset::inst::push_u32(reinterpret_cast<void*>(0x00619929), reinterpret_cast<uintptr_t>(&BASE_DATA_DIRECTORY_NAME_C_FMT));

	if (save_redirection_directory.string().length() >= 256) {
		std::string save_redirection_string = save_redirection_directory.string();
		LOG_LOCALIZED_STRING(SAVE_DIR_TOO_LARGE, save_redirection_string);
	}

	if (!mods_enabled.empty()) {
		LOADED_FS.collect_files(mods_enabled);
	}
	if (LOADED_FS.save_file_redirection_enabled) {
		LOADED_FS.save_redirection_directory = save_redirection_directory;
	}
	else {
		wchar_t documents[1024];
		SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, documents);
		LOADED_FS.save_redirection_directory = std::filesystem::path(documents) / L"\\THQ\\Cars2\\";
	}

	TRCEngineGetSaveDir::install_at_ptr(0x00425280);

	fopenHook::install_at_ptr(0x0063FCBF);

	sunset::utils::set_permission(reinterpret_cast<void*>(0x006742E8), 4, sunset::utils::Perm::ReadWrite);
	*reinterpret_cast<void**>(0x006742E8) = BinkOpenHook;

	sunset::utils::set_permission(reinterpret_cast<void*>(0x00674084), 4, sunset::utils::Perm::ReadWrite);
	*reinterpret_cast<void**>(0x00674084) = BASS_SampleLoadHook;

	sunset::utils::set_permission(reinterpret_cast<void*>(0x00674040), 4, sunset::utils::Perm::ReadWrite);
	*reinterpret_cast<void**>(0x00674040) = BASS_StreamCreateFileHook;

	return true;
}