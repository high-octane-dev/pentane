#pragma once
#include <filesystem>
#include <format>
#include <string>
#include "localization.hpp"
#include "config.hpp"

namespace logger {
	bool init(const std::filesystem::path& file_path, bool create_console, bool log_to_file);

	void log(const std::string_view&);

	template<typename... Args> void log_format(std::format_string<Args...> fmt, Args&&... args) {
		auto str = std::vformat(fmt.get(), std::make_format_args(args...));
		log(str);
	}

	template <typename... Args> void log_localized(const std::array<std::string_view, PentaneLanguage::Max>& label, Args&&... args) {
		auto format_str = label[static_cast<size_t>(config::language())];
		auto formatted = std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...));
		log(formatted);
	}

	inline void log_localized(const std::array<std::string_view, PentaneLanguage::Max>& label) {
		log(label[static_cast<size_t>(config::language())]);
	}
};

#define LOG_LOCALIZED_STRING(LABEL, ...) logger::log_localized(LABEL, __VA_ARGS__)