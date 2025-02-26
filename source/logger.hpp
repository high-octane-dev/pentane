#pragma once
#include <filesystem>
#include <format>
#include <string>

namespace logger {
	bool init(const std::filesystem::path& file_path, bool create_console, bool log_to_file);

	void log(const std::string&);

	template<typename... Args>
	void log_format(std::format_string<Args...> fmt, Args&&... args) {
		auto str = std::vformat(fmt.get(), std::make_format_args(args...));
		log(str);
	}
};