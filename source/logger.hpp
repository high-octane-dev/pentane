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

	template <typename... Args> void log_localized(const FullyLocalizedString& label, Args&&... args) {
		std::string_view format_str = label[static_cast<size_t>(config::language())];
		std::string formatted = std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...));
		log(formatted);
	}

	inline void log_localized(const FullyLocalizedString& label) {
		log(label[static_cast<size_t>(config::language())]);
	}

	template <typename... Args> void log_localized(const LocalizedString& label, Args&&... args) {
		std::string_view format_str = localization::get_with_fallback(label, config::language());
		std::string formatted = std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...));
		log(formatted);
	}

	inline void log_localized(const LocalizedString& label) {
		log(localization::get_with_fallback(label, config::language()));
	}
};

#define LOG_LOCALIZED_STRING(LABEL, ...) logger::log_localized(LABEL, __VA_ARGS__)