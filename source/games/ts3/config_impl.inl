
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

bool config::ts3::windowed_mode_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->windowed_mode_enabled();
}
