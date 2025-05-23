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

void __stdcall RedirectToLogger(const char* message) {
	if (message != nullptr) {
		std::string_view string = message;
		while (string.ends_with('\n')) {
			string.remove_suffix(1);
		}
		logger::log_format("[OutputDebugStringA] {}", string);
	}
}

void _cdecl RedirectDbgPrint(const char* format,...) {
	char buffer[1024]{};
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);
	std::string_view string = buffer;
	while (string.ends_with('\n')) {
		string.remove_suffix(1);
	}
	logger::log_format("[DbgPrint] {}", string);
}

DefineReplacementHook(HandleLog) {
	static void __fastcall callback(void* _this, std::uintptr_t edx, char* message) {
		if (message != nullptr) {
			std::string_view string = message;
			while (string.ends_with('\n')) {
				string.remove_suffix(1);
			}
			logger::log_format("[CommandConsole::Echo] {}", string);
		}
	}
};

BOOL WINAPI DllMain(HINSTANCE instance_handle, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		// In Cars 3: Driven to Win, we're being loaded by `kernelx.dll` in the WinDurango runtime, and as such, don't need to hijack another module.
#if !defined(PENTANE_GAME_TARGET_3DTW)
		bink::replace_funcs();
#endif
		std::filesystem::path install_dir = set_current_directory_to_module_location();

		std::vector<std::string_view> config_initialization_errors{};
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
		// 2TVG-Specific initialization.
		// Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
		sunset::inst::nop(reinterpret_cast<void*>(0x007b599f), 6);

		// Redirects DoDbgPrint to the logger.
		sunset::inst::jmp(reinterpret_cast<void*>(0x005ef280), RedirectDbgPrint);
		
		// Initialize the filesystem.
		tvg2::fs::init();
#elif defined(PENTANE_GAME_TARGET_2TVGA)
		// 2TVGA-Specific initialization.
		// Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
		sunset::inst::nop(reinterpret_cast<void*>(0x008249cf), 6);

		// Redirects OutputDebugStringA to the logger.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0159113c), sizeof(void*), sunset::utils::Perm::ReadWrite);
		*reinterpret_cast<void**>(0x0159113c) = RedirectToLogger;

		// Redirects the CommandConsole to the logger.
		HandleLog::install_at_ptr(0x00ba4bf0);

		// Redirects DoDbgPrint to the logger.
		sunset::inst::jmp(reinterpret_cast<void*>(0x00be5120), RedirectDbgPrint);
		
		// Initialize the filesystem.
		tvg2::fs::init();
#endif
	}
	return TRUE;
}