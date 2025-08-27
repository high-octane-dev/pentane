#include "file_system.hpp"
#include "../../sunset/sunset.hpp"
#include "../../logger.hpp"
#include "../../target.hpp"

#if defined(PENTANE_GAME_TARGET_2TVGA)
auto _DownloadManager_DownloadManager = (std::uint32_t(__thiscall*)(class DownloadManager*))(0x00791140);
auto _DownloadManager_Destructor = (std::uint32_t(__thiscall*)(class DownloadManager*))(0x007911e0);
auto _DownloadManager_Update = (void(__thiscall*)(class DownloadManager*))(0x00791280);
#elif defined(PENTANE_GAME_TARGET_2TVG)
auto _DownloadManager_DownloadManager = (std::uint32_t(__thiscall*)(class DownloadManager*))(0x00747b70);
auto _DownloadManager_Destructor = (std::uint32_t(__thiscall*)(class DownloadManager*))(0x00747c10);
auto _DownloadManager_Update = (void(__thiscall*)(class DownloadManager*))(0x00747cb0);
#endif

namespace Genie {
    class VirtualDriveManager;
    class VirtualDrive;
    class VirtualFileHandle;
};

DefineReplacementHook(OpenFileHook) {
    static unsigned int _cdecl callback(const char* path, struct HALFileWin32** out_handle, unsigned int write_enable) {
        if ((write_enable & 1) == 0) {
            if (std::string_view(path).contains("startup")) {
                std::rand();
            }
            logger::log_format("[MediaHAL::Open] Opening File (R): {}", std::string_view(path));
        }
        else if ((write_enable & 1) != 0) {
            logger::log_format("[MediaHAL::Open] Opening File (W): {}", std::string_view(path));
        }
        return original(path, out_handle, write_enable);
    }
};

DefineReplacementHook(OpenVirtualDrive) {
    static Genie::VirtualDrive* _fastcall callback(Genie::VirtualDriveManager* _this, std::uintptr_t edx, const char** path, bool create, struct File* givenFile, bool lock) {
        logger::log_format("[Genie::VirtualDriveManager::Open] Opening Virtual Drive: {}", std::string_view(*path));
        return original(_this, edx, path, create, givenFile, lock);
    }
};

DefineReplacementHook(OpenVirtualFile) {
    static bool _fastcall callback(Genie::VirtualDrive* _this, std::uintptr_t edx, const char* path, Genie::VirtualFileHandle* fileHandle) {
        bool result = original(_this, edx, path, fileHandle);
        if (result) {
            logger::log_format("[Genie::VirtualDrive::FileOpen] Opened Virtual File: {}", std::string_view(path));
        }
        else {
            logger::log_format("[Genie::VirtualDrive::FileOpen] Failed to open Virtual File: {}", std::string_view(path));
        }
        return result;
    }
};

struct DownloadManager {
    int mState;
    char unused[0x100];
    inline DownloadManager() : mState(0), unused() {
        _DownloadManager_DownloadManager(this);
    }
    inline ~DownloadManager() {
        _DownloadManager_Destructor(this);
    }
    inline void Update() {
        _DownloadManager_Update(this);
    }
    DownloadManager(const DownloadManager&) = delete;
    DownloadManager& operator=(const DownloadManager&) = delete;
};

std::unique_ptr<DownloadManager> g_DownloadManager = nullptr;

DefineInlineHook(InitDownloadManager) {
    static void __cdecl callback(sunset::InlineCtx & ctx) {
        g_DownloadManager = std::make_unique<DownloadManager>();
    }
};

DefineInlineHook(UpdateDownloadManager) {
    static void __cdecl callback(sunset::InlineCtx & ctx) {
        g_DownloadManager->Update();
    }
};

auto tvg2::fs::init() -> bool {
    if constexpr (GAME_TARGET == PentaneTarget::Cars2TheVideoGameArcade) {
        // OpenFileHook::install_at_ptr(0x007680b0);
        // OpenVirtualDrive::install_at_ptr(0x00711570);
        // OpenVirtualFile::install_at_ptr(0x00071c1b0);
        InitDownloadManager::install_at_ptr(0x0080d08b);
        UpdateDownloadManager::install_at_ptr(0x004bcf40);

        // Raises the maximum size allocated for the global character database in `CarManager` to allow for 1024 entries in `carsinfo.lua`.
        sunset::inst::push_u32(reinterpret_cast<void*>(0x004d58f7), 0x22000);
    }
    else {
        // OpenFileHook::install_at_ptr(0x0071ebd0);
        // OpenVirtualDrive::install_at_ptr(0x006cec70);
        // OpenVirtualFile::install_at_ptr(0x006d98b0);
        InitDownloadManager::install_at_ptr(0x007a44f5);
        UpdateDownloadManager::install_at_ptr(0x004877b0);

        // Raises the maximum size allocated for the global character database in `CarManager` to allow for 1024 entries in `carsinfo.lua`.
        sunset::inst::push_u32(reinterpret_cast<void*>(0x004a8bd4), 0x22000);
    }
    return false;
}