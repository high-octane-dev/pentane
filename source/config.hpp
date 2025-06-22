#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <utility>
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
		std::pair<int, int> windowed_mode_dimensions();
	};
#endif
};