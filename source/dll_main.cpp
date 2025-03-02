#include <iostream>
#include <Windows.h>
#include <pathcch.h>
#include "bink/bink.hpp"
#include "config.hpp"
#include "games/mn/mn_file_system.hpp"
#include "games/tvg/tvg_file_system.hpp"
#include "games/tvg2/tvg2_file_system.hpp"
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

BOOL WINAPI DllMain(HINSTANCE instance_handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        bink::replace_funcs();
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
        tvg::fs::init();
#elif defined(PENTANE_GAME_TARGET_2TVG)
        // TVG2-Specific initialization.
        // Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
        sunset::write_nop(reinterpret_cast<void*>(0x007b599f), 6);

        tvg2::fs::init();
#elif defined(PENTANE_GAME_TARGET_2TVGA)
        // TVG2A-Specific initialization.
        // Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
        sunset::write_nop(reinterpret_cast<void*>(0x008249cf), 6);

        tvg2::fs::init();
#endif
    }
    return TRUE;
}