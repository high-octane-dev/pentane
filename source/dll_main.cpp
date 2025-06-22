#include <iostream>
#include <Windows.h>
#include <pathcch.h>
#include <shlwapi.h>

#if !defined(PENTANE_GAME_TARGET_3DTW)
#include "bink/bink.hpp"
#endif

#include "config.hpp"
#include "crash.hpp"
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

auto get_running_game_from_module_timestamp() -> PentaneTarget {
	const auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr)) + reinterpret_cast<IMAGE_DOS_HEADER*>(GetModuleHandleW(nullptr))->e_lfanew);
	switch (nt_header->FileHeader.TimeDateStamp) {
	case 0x445A4258:
		return PentaneTarget::CarsTheVideoGame;
	case 0x47160288:
		return PentaneTarget::CarsMaterNationalChampionship;
	case 0x4DDE6899:
		return PentaneTarget::Cars2TheVideoGame;
	case 0x521E2EAF:
		return PentaneTarget::Cars2TheVideoGameArcade;
	case 0x58F94AF7:
		return PentaneTarget::Cars3DrivenToWin;
	default:
		return PentaneTarget::Invalid;
		break;
	}
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
		// Check to make sure the GAME_TARGET matches the game that's actually running.
		auto running_game = get_running_game_from_module_timestamp();
		if (running_game == PentaneTarget::Invalid) {
			logger::log_localized(FAILED_READ_TIMESTAMP, GAME_TARGET);
			display_error(localization::get_with_fallback(FAILED_READ_TIMESTAMP, config::language()), localization::get_with_fallback(ERROR_POPUP_TITLE, config::language()));
			std::abort();
		}
		else if (running_game != GAME_TARGET) {
			logger::log_localized(TARGET_MISMATCH, running_game, GAME_TARGET);
			display_error(localization::get_with_fallback(TARGET_MISMATCH, config::language()), localization::get_with_fallback(ERROR_POPUP_TITLE, config::language()));
			std::abort();
		}
		for (const auto& message : config_initialization_errors) {
			logger::log(message);
			display_error(message, localization::get_with_fallback(ERROR_POPUP_TITLE, config::language()));
		}
		// Once we've initialized the logger, it's now safe to set up the exception handler.
		crash_handler::install();
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
		if (config::console_logging_enabled()) {
			// Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
			sunset::inst::nop(reinterpret_cast<void*>(0x007b599f), 6);
		}

		if (config::tvg2::windowed_mode_enabled()) {
			// Enables windowed mode in Renderer::DriverImpl::DoCreateWindow.
			sunset::utils::set_permission(reinterpret_cast<void*>(0x007C56A4), 1, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint8_t*>(0x007C56A4) = 0;
		}

		// Redirects DoDbgPrint to the logger.
		sunset::inst::jmp(reinterpret_cast<void*>(0x005ef280), RedirectDbgPrint);
		
		// Initialize the filesystem.
		tvg2::fs::init();
#elif defined(PENTANE_GAME_TARGET_2TVGA)
		// 2TVGA-Specific initialization.
		if (config::console_logging_enabled()) {
			// Removes a `FreeConsole` call from the original game, allowing the console logger to function correctly.
			sunset::inst::nop(reinterpret_cast<void*>(0x008249cf), 6);
		}

		if (config::tvg2::windowed_mode_enabled()) {
			// Enables windowed mode in Renderer::DriverConfiguration::DriverConfiguration.
			sunset::utils::set_permission(reinterpret_cast<void*>(0x0088B77C), 1, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint8_t*>(0x0088B77C) = 0;
			// Sets the window dimensions. The game will automatically search for a valid display mode if the supplied resolution is invalid.
			auto [w, h] = config::tvg2::windowed_mode_dimensions();
			sunset::utils::set_permission(reinterpret_cast<void*>(0x00d70b51), 4, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint32_t*>(0x00d70b51) = static_cast<std::uint32_t>(w);
			sunset::utils::set_permission(reinterpret_cast<void*>(0x00d70b58), 4, sunset::utils::Perm::ExecuteReadWrite);
			*reinterpret_cast<std::uint32_t*>(0x00d70b58) = static_cast<std::uint32_t>(h);
		}

		// Redirects OutputDebugStringA to the logger.
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0159113c), sizeof(void*), sunset::utils::Perm::ReadWrite);
		*reinterpret_cast<void**>(0x0159113c) = RedirectToLogger;

		// Redirects the CommandConsole to the logger.
		HandleLog::install_at_ptr(0x00ba4bf0);

		// Redirects DoDbgPrint to the logger.
		sunset::inst::jmp(reinterpret_cast<void*>(0x00be5120), RedirectDbgPrint);

		// Prevents the game from generating HTML log files.
		sunset::inst::nop(reinterpret_cast<void*>(0x0075fb27), 0x1C);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0075fb27), 5, sunset::utils::Perm::ExecuteReadWrite);
		*reinterpret_cast<std::uint8_t*>(0x0075fb27) = 0xB8;
		*reinterpret_cast<std::uint32_t*>(0x0075fb28) = 0xFFFFFFFF;

		// Initialize the filesystem.
		tvg2::fs::init();
#endif
	}
	return TRUE;
}