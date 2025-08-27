#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <utility>
#include <cstdint>
#include "localization.hpp"

namespace config {
	bool init_global(const std::filesystem::path& file_path, std::vector<std::string_view>& errors);
	PentaneLanguage language();
	bool console_logging_enabled();
	bool file_logging_enabled();
	std::vector<std::string> mods_enabled();
	std::vector<std::string> plugins_enabled();
#if defined(PENTANE_GAME_TARGET_MN)
	namespace mn {
		bool save_redirection_enabled();
		std::string data_directory_name();
	};
#elif defined(PENTANE_GAME_TARGET_2TVG)
	namespace tvg2 {
		bool windowed_mode_enabled();
	};
#elif defined(PENTANE_GAME_TARGET_2TVGA)
	namespace tvg2 {
		bool windowed_mode_enabled();
		std::pair<std::int64_t, std::int64_t> windowed_mode_dimensions();
	};
#elif defined(PENTANE_GAME_TARGET_TS3)
	namespace ts3 {
		bool windowed_mode_enabled();
	};
#endif
};

auto display_error(std::string_view error, std::string_view title) -> void;

template <typename... Args> void display_error_formatted(const LocalizedString& label, const LocalizedString& title, Args&&... args) {
	std::string_view format_str = localization::get_with_fallback(label, config::language());
	std::string_view title_str = localization::get_with_fallback(title, config::language());
	std::string formatted = std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...));
	display_error(formatted, title_str);
}