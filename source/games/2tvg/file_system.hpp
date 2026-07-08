#pragma once
#include <filesystem>
namespace tvg2 {
	namespace fs {
		auto init(const std::filesystem::path& install_directory,
			const std::filesystem::path& mods_directory,
			const std::vector<std::string>& mods_enabled) -> bool;
	};
};