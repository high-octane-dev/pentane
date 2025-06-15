
// Steam Implementation
#if defined(PENTANE_GAME_TARGET_2TVG)
class GameConfig : public GlobalConfig {
private:
	bool enable_windowed_mode = false;
public:
	auto read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool;
	auto init(const std::filesystem::path& file_path, std::vector<std::string_view>& errors) -> bool;
	auto windowed_mode_enabled() -> bool;
};

auto GameConfig::read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool {
	GlobalConfig::read(tbl, errors);
	if (tbl.contains("game-config")) {
		const auto& game_config_node = tbl["game-config"];

		if (!game_config_node.as_table()->contains("enable_windowed_mode")) {
			errors.push_back(localization::get_with_fallback(TVG2_CONFIG_MISSING_ENABLE_WINDOWED_MODE, get_language()));
		}
		else {
			enable_windowed_mode = game_config_node["enable_windowed_mode"].as_boolean()->get();
		}
	}
	else {
		errors.push_back(localization::get_with_fallback(TVG2_CONFIG_MISSING, get_language()));
		return false;
	}
	return true;
}

auto GameConfig::init(const std::filesystem::path& file_path, std::vector<std::string_view>& errors) -> bool
{
	toml::table tbl{};
	try {
		tbl = toml::parse_file(file_path.string());
	}
	catch (const toml::parse_error& err) {
		// This is the only other place that we use the system language; this time to let the user know that we failed to parse `config.toml`.
		errors.push_back(localization::get_with_fallback(TVG2_CONFIG_PARSE_FAIL, get_system_language()));
		return false;
	}
	return read(tbl, errors);
}

auto GameConfig::windowed_mode_enabled() -> bool {
	return enable_windowed_mode;
}

bool config::tvg2::windowed_mode_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->windowed_mode_enabled();
}

#elif defined(PENTANE_GAME_TARGET_2TVGA)
// Arcade Implementation
class GameConfig : public GlobalConfig {
private:
	bool enable_windowed_mode = false;
	int desired_window_width = 1280;
	int desired_window_height = 720;
public:
	auto read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool;
	auto init(const std::filesystem::path& file_path, std::vector<std::string_view>& errors) -> bool;
	auto windowed_mode_enabled() -> bool;
	auto window_dimensions() -> std::pair<int, int>;
};

auto GameConfig::read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool {
	GlobalConfig::read(tbl, errors);
	if (tbl.contains("game-config")) {
		const auto& game_config_node = tbl["game-config"];

		if (!game_config_node.as_table()->contains("enable_windowed_mode")) {
			errors.push_back(localization::get_with_fallback(TVG2_CONFIG_MISSING_ENABLE_WINDOWED_MODE, get_language()));
		}
		else {
			enable_windowed_mode = game_config_node["enable_windowed_mode"].as_boolean()->get();
		}

		bool found_width = false, found_height = false;

		if (!game_config_node.as_table()->contains("desired_window_width")) {
			errors.push_back(localization::get_with_fallback(TVG2A_CONFIG_MISSING_WINDOW_WIDTH, get_language()));
		}
		else {
			found_width = true;
			desired_window_width = game_config_node["desired_window_width"].as_integer()->get();
		}

		if (!game_config_node.as_table()->contains("desired_window_height")) {
			errors.push_back(localization::get_with_fallback(TVG2A_CONFIG_MISSING_WINDOW_HEIGHT, get_language()));
		}
		else {
			found_height = true;
			desired_window_height = game_config_node["desired_window_height"].as_integer()->get();
		}

		// If we found a width but not a height, or vice versa, or if the user selected a negative resolution then fall back to 720p.
		if ((found_width && !found_height) || (found_height && !found_width) || desired_window_height < 0 || desired_window_height < 0) {
			desired_window_width = 1280;
			desired_window_height = 720;
		}
	}
	else {
		errors.push_back(localization::get_with_fallback(TVG2A_CONFIG_MISSING, get_language()));
		return false;
	}
	return true;
}

auto GameConfig::init(const std::filesystem::path& file_path, std::vector<std::string_view>& errors) -> bool
{
	toml::table tbl{};
	try {
		tbl = toml::parse_file(file_path.string());
	}
	catch (const toml::parse_error& err) {
		// This is the only other place that we use the system language; this time to let the user know that we failed to parse `config.toml`.
		errors.push_back(localization::get_with_fallback(TVG2A_CONFIG_PARSE_FAIL, get_system_language()));
		return false;
	}
	return read(tbl, errors);
}

auto GameConfig::windowed_mode_enabled() -> bool {
	return enable_windowed_mode;
}

auto GameConfig::window_dimensions() -> std::pair<int, int> {
	return std::make_pair(desired_window_width, desired_window_height);
}

bool config::tvg2::windowed_mode_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->windowed_mode_enabled();
}

std::pair<int, int> config::tvg2::window_dimensions() {
	const auto guard = CONFIG.lock();
	return (*guard)->window_dimensions();
}

#endif