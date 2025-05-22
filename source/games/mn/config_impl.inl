
class GameConfig : public GlobalConfig {
private:
	bool enable_save_redirection = false;
	std::string data_directory_name = "DataPC";
public:
	auto read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool;
	auto init(const std::filesystem::path& file_path, std::vector<std::string_view>& errors) -> bool;
	auto save_redirection_enabled() const -> bool;
	auto data_directory() const->std::string;
};


auto GameConfig::read(const toml::table& tbl, std::vector<std::string_view>& errors) -> bool {
	GlobalConfig::read(tbl, errors);
	if (tbl.contains("game-config")) {
		const auto& game_config_node = tbl["game-config"];

		if (!game_config_node.as_table()->contains("enable_save_redirection")) {
			errors.push_back(MATER_NATIONAL_CONFIG_MISSING_ENABLE_SAVE_REDIRECTION[get_language()]);
		}
		else {
			enable_save_redirection = game_config_node["enable_save_redirection"].as_boolean()->get();
		}

		if (!game_config_node.as_table()->contains("data_directory_name")) {
			errors.push_back(MATER_NATIONAL_CONFIG_MISSING_DATA_DIRECTORY[get_language()]);
		}
		else {
			data_directory_name = game_config_node["data_directory_name"].as_string()->get();
		}
	}
	else {
		errors.push_back(MATER_NATIONAL_CONFIG_MISSING[get_language()]);
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
		errors.push_back(CONFIG_PARSE_FAIL[get_system_language()]);
		return false;
	}
	return read(tbl, errors);
}

auto GameConfig::save_redirection_enabled() const -> bool {
	return enable_save_redirection;
}

auto GameConfig::data_directory() const -> std::string {
	return data_directory_name;
}

bool config::mn::save_redirection_enabled() {
	const auto guard = CONFIG.lock();
	return (*guard)->save_redirection_enabled();
}

std::string config::mn::data_directory_name() {
	const auto guard = CONFIG.lock();
	return (*guard)->data_directory();
}