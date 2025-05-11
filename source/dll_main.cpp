#include <iostream>
#include <Windows.h>
#include <pathcch.h>
#include <shlwapi.h>

#if !defined(PENTANE_GAME_TARGET_3DTW)
#include "bink/bink.hpp"
#endif
#include "config.hpp"
#include "games/mn/file_system.hpp"
#include "games/tvg/file_system.hpp"
#include "games/2tvg/file_system.hpp"
#include "logger.hpp"
#include "plugin_loader.hpp"
#include "target.hpp"
#include "sunset/sunset.hpp"

auto set_current_directory_to_module_location() -> std::filesystem::path {
    wchar_t game_directory[1024];
    GetModuleFileNameW(nullptr, game_directory, 1024);
    PathCchRemoveFileSpec(game_directory, 1024);
    CharLowerW(game_directory);
    SetCurrentDirectoryW(game_directory);
    return game_directory;
}

#if defined(PENTANE_GAME_TARGET_2TVG) or defined(PENTANE_GAME_TARGET_2TVGA)
auto is_arcade() -> bool {
    wchar_t exe_name[1024];
    GetModuleFileNameW(nullptr, exe_name, 1024);
    PathStripPathW(exe_name);
    if (std::wstring_view(exe_name).contains(L"daemon")) {
        return true;
    }
    return false;
}
#endif

BOOL WINAPI DllMain(HINSTANCE instance_handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        // In Cars 3: Driven to Win, we're being loaded by `kernelx.dll` in the WinDurango runtime, and as such, don't need to hijack another module.
#if !defined(PENTANE_GAME_TARGET_3DTW)
        bink::replace_funcs();
#endif
        std::filesystem::path install_dir = set_current_directory_to_module_location();
        
        std::vector<std::string> config_initialization_errors{};
        config::init_global(install_dir / "Pentane\\config.toml", config_initialization_errors);
        logger::init(install_dir / "pentane.log", config::console_logging_enabled(), config::file_logging_enabled());
        for (const auto& message : config_initialization_errors) {
            logger::log(message);
        }
        plugin_loader::load_all(install_dir / "Pentane\\Plugins");
#if defined(PENTANE_GAME_TARGET_MN)
        // MN-Specific initialization.
        if (install_dir.native().contains(L".")) {
            logger::log("[DllMain] Cars Mater-National is known to have issues when ran from a directory that contains a period. Please consider renaming your install directory!");
        }
        mn::fs::init(config::mn::save_redirection_enabled(),
            install_dir,
            install_dir / "Pentane\\SaveData",
            install_dir / config::mn::data_directory_name(),
            install_dir / "Pentane\\Mods",
            config::mods_enabled());
#elif defined(PENTANE_GAME_TARGET_TVG)
        // TVG-Specific initialization.
        tvg::fs::init(install_dir,
            install_dir / "Data",
            install_dir / "Pentane\\Mods",
            config::mods_enabled());

#elif defined(PENTANE_GAME_TARGET_2TVG)
        auto arcade = is_arcade();
        if (arcade) {
            // TVG2A-Specific initialization.
            // Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
            sunset::inst::nop(reinterpret_cast<void*>(0x008249cf), 6);
        }
        else {
            // TVG2-Specific initialization.
            // Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
            sunset::inst::nop(reinterpret_cast<void*>(0x007b599f), 6);
        }
        tvg2::fs::init();
#endif
    }
    return TRUE;
}