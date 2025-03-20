#pragma once
namespace mn {
	namespace fs {
		auto init(bool enable_save_redirection,
			const std::filesystem::path& install_directory,
			const std::filesystem::path& save_redirection_directory,
			const std::filesystem::path& base_data_directory,
			const std::filesystem::path& mods_directory,
			const std::vector<std::string>& mods_enabled) -> bool;
		auto resolve_path(std::string path) -> std::filesystem::path;
		auto save_dir() -> std::filesystem::path;
	};
};