#include <memory>
#include <utility>
#include <mutex>
#include <cstdint>
#include <toml++/toml.hpp>
#include <Windows.h>

#include "target.hpp"

#include "config.hpp"
#include "util/mutex.hpp"

bool ichar_equals(char a, char b) {
	return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
}

bool iequals(std::string_view lhs, std::string_view rhs) {
	return std::ranges::equal(lhs, rhs, ichar_equals);
}

PentaneLanguage get_system_language() {
	char language_iso639_1[1024]{};
	GetLocaleInfoA(GetUserDefaultUILanguage(), LOCALE_SISO639LANGNAME, language_iso639_1, 1024);
	for (std::size_t i = 0; i < PentaneLanguage::Max; i++) {
		if (LANGUAGE_CODE[i] == language_iso639_1) {
			return static_cast<PentaneLanguage>(i);
		}
	}
	return PentaneLanguage::English;
}

auto display_error(std::string_view error, std::string_view title) -> void {
	MessageBoxA(nullptr, error.data(), title.data(), MB_ICONERROR);
}

class GlobalConfig {
private:
	PentaneLanguage language = PentaneLanguage::English;
	bool enable_console_logging = true;
	bool enable_file_logging = true;
	std::vector<std::string> enabled_mods;
	std::vector<std::string> enabled_plugins;
public:
	auto read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool;
	auto get_language() const -> PentaneLanguage;
	auto console_logging_enabled() const -> bool;
	auto file_logging_enabled() const -> bool;
	auto get_enabled_mods() const -> std::vector<std::string>;
	auto get_enabled_plugins() const -> std::vector<std::string>;
};

/*
// Every Game-specific configuration class must implement the two functions `read` and `init`.

class GameConfig : public GlobalConfig {
public:
	auto read(const toml::table& tbl, std::vector<std::string>& errors) -> bool;
	auto init(const std::filesystem::path& file_path, std::vector<std::string>& errors) -> bool;
};
*/

class GameConfig;

util::Mutex<std::unique_ptr<GameConfig>> CONFIG;

#if defined(PENTANE_GAME_TARGET_TVG)
#include "games/tvg/config_impl.inl"
#elif defined(PENTANE_GAME_TARGET_MN)
#include "games/mn/config_impl.inl"
#elif defined(PENTANE_GAME_TARGET_2TVG) || defined(PENTANE_GAME_TARGET_2TVGA)
#include "games/2tvg/config_impl.inl"
#elif defined(PENTANE_GAME_TARGET_3DTW)
#include "games/3dtw/config_impl.inl"
#endif

auto GlobalConfig::read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool {
	// We'll use the system language for the first error message, since we can't know the user-selected language at that point.
	// We'll only use the system language to print CONFIG_PARSE_FAIL, GLOBAL_CONFIG_CONFIG_MISSING_LANG, or GLOBAL_CONFIG_MISSING_CONFIG; if either `config` is missing or `language` is missing in `config.toml`.
	PentaneLanguage system_language = get_system_language();
	if (tbl.contains("config")) {
		const auto& config_node = tbl["config"];

		std::string language_string{ LANGUAGE_NAME[PentaneLanguage::English] };
		bool should_load_mods = false;
		bool should_load_plugins = false;

		if (!config_node.as_table()->contains("language")) {
			errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_CONFIG_MISSING_LANG, system_language));
		}
		else {
			language_string = config_node["language"].as_string()->get();
		}

		bool found_language = false;
		for (std::size_t i = 0; i < PentaneLanguage::Max; i++) {
			if (iequals(LANGUAGE_CODE[i], language_string)) {
				language = static_cast<PentaneLanguage>(i);
				found_language = true;
				break;
			}
			if (iequals(LANGUAGE_CODE_3[i], language_string)) {
				language = static_cast<PentaneLanguage>(i);
				found_language = true;
				break;
			}
			if (iequals(LANGUAGE_EN[i], language_string)) {
				language = static_cast<PentaneLanguage>(i);
				found_language = true;
				break;
			}
			if (LANGUAGE_NAME_LOWER[i] == language_string) {
				language = static_cast<PentaneLanguage>(i);
				found_language = true;
				break;
			}
			if (LANGUAGE_NAME[i] == language_string) {
				language = static_cast<PentaneLanguage>(i);
				found_language = true;
				break;
			}
		}

		if (!found_language) {
			errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_CONFIG_UNRECOGNIZED_LANG, system_language));
		}
		else if (!localization::support_status(language)) {
			errors.push_back(INCOMPLETE_LOCALIZATION[language]);
		}
		
		// Pentane's OE mod loader is not yet implemented.
		if constexpr (!util::octane()) {
			if (!config_node.as_table()->contains("enable_mods")) {
				errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_MODS, language));
			}
			else {
				should_load_mods = config_node["enable_mods"].as_boolean()->get();
			}
		}
		
		if (!config_node.as_table()->contains("enable_plugins")) {
			errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_PLUGINS, language));
		}
		else {
			should_load_plugins = config_node["enable_plugins"].as_boolean()->get();
		}

		if (!config_node.as_table()->contains("enable_console_logging")) {
			errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_CONSOLE_LOGGING, language));
		}
		else {
			enable_console_logging = config_node["enable_console_logging"].as_boolean()->get();
		}

		if (!config_node.as_table()->contains("enable_file_logging")) {
			errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_FILE_LOGGING, language));
		}
		else {
			enable_file_logging = config_node["enable_file_logging"].as_boolean()->get();
		}

		if (should_load_mods) {
			if (tbl.contains("mods")) {
				if (tbl["mods"].as_table()->contains("enabled_mods")) {
					const auto& enabled_mods_array = tbl["mods"]["enabled_mods"].as_array();
					for (std::size_t i = 0; i < enabled_mods_array->size(); i++) {
						enabled_mods.push_back(enabled_mods_array->get(i)->as_string()->get());
					}
				}
				else {
					errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_MODS_MISSING_ENABLED_MODS, language));
				}
			}
			else {
				errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_MISSING_MODS, language));
			}
		}
		if (should_load_plugins) {
			if (tbl.contains("plugins")) {
				if (tbl["plugins"].as_table()->contains("enabled_plugins")) {
					const auto& enabled_plugins_array = tbl["plugins"]["enabled_plugins"].as_array();
					for (std::size_t i = 0; i < enabled_plugins_array->size(); i++) {
						enabled_plugins.push_back(enabled_plugins_array->get(i)->as_string()->get());
					}
				}
				else {
					errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_PLUGINS_MISSING_ENABLED_PLUGINS, language));
				}
			}
			else {
				errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_MISSING_PLUGINS, language));
			}
		}
	}
	else {
		errors.push_back(localization::get_with_fallback(GLOBAL_CONFIG_MISSING_CONFIG, system_language));
		return false;
	}
	return true;
}

auto GlobalConfig::get_language() const -> PentaneLanguage {
	return language;
}

auto GlobalConfig::console_logging_enabled() const -> bool {
	return enable_console_logging;
}

auto GlobalConfig::file_logging_enabled() const -> bool{
	return enable_file_logging;
}

auto GlobalConfig::get_enabled_mods() const -> std::vector<std::string> {
	return enabled_mods;
}

auto GlobalConfig::get_enabled_plugins() const -> std::vector<std::string> {
	return enabled_plugins;
}


bool config::init_global(const std::filesystem::path& file_path, std::vector<std::string_view>& errors) {
	auto guard = CONFIG.lock_mut();
	*guard = std::make_unique<GameConfig>();
	return (*guard)->init(file_path, errors);
}

PentaneLanguage config::language() {
	const auto guard = CONFIG.lock();
	return (*guard)->get_language();
}

bool config::console_logging_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->console_logging_enabled();
}

bool config::file_logging_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->file_logging_enabled();
}

std::vector<std::string> config::mods_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->get_enabled_mods();
}

std::vector<std::string> config::plugins_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->get_enabled_plugins();
}
