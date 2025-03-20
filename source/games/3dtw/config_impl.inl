class GameConfig : public GlobalConfig {
public:
	auto read(const toml::table& tbl, std::vector<std::string>& errors) -> bool;
	auto init(const std::filesystem::path& file_path, std::vector<std::string>& errors) -> bool;
};

auto GameConfig::read(const toml::table& tbl, std::vector<std::string>& errors) -> bool {
	GlobalConfig::read(tbl, errors);
	return true;
}

auto GameConfig::init(const std::filesystem::path& file_path, std::vector<std::string>& errors) -> bool
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