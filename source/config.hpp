#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include "localization.hpp"

namespace config {
	bool init_global(const std::filesystem::path& file_path, std::vector<std::string>& errors);
	PentaneLanguage language();
	bool console_logging_enabled();
	bool file_logging_enabled();
	std::vector<std::string> mods_enabled();
	std::vector<std::string> plugins_enabled();
	namespace mn {
		bool save_redirection_enabled();
		std::string data_directory_name();
	};
};