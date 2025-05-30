#pragma once
#include <string>
#include <array>

// #define LOG_LOCALIZED_STRING(LABEL, ...) { std::string value = std::vformat(LABEL[config::language()], std::make_format_args(__VA_ARGS__)); logger::log(value); }

enum PentaneLanguage {
	// NA
	English,
	Spanish,
	French,
	// EU
	German,
	Polish,
	Russian,
	// AS
	Japanese,
	Korean,
	Hindi,
	// AF

	Max
};

constexpr std::uint16_t GET_LANGUAGE_CODE_EN = 'e' << 8 | 'n';
constexpr std::uint16_t GET_LANGUAGE_CODE_ES = 'e' << 8 | 's';
constexpr std::uint16_t GET_LANGUAGE_CODE_FR = 'f' << 8 | 'r';
constexpr std::uint16_t GET_LANGUAGE_CODE_DE = 'd' << 8 | 'e';
constexpr std::uint16_t GET_LANGUAGE_CODE_PL = 'p' << 8 | 'l';
constexpr std::uint16_t GET_LANGUAGE_CODE_RU = 'r' << 8 | 'u';
constexpr std::uint16_t GET_LANGUAGE_CODE_JA = 'j' << 8 | 'a';
constexpr std::uint16_t GET_LANGUAGE_CODE_KO = 'k' << 8 | 'o';
constexpr std::uint16_t GET_LANGUAGE_CODE_HI = 'h' << 8 | 'i';

// 2-letter latin codes for each language, as per ISO 639-1.
inline std::array<std::string_view, PentaneLanguage::Max> LANGUAGE_CODE {
	"en", "es", "fr", "de", "pl", "ru", "ja", "ko", "hi"
};

// 3-letter latin codes for each language, as per ISO 639-2.
inline std::array<std::string_view, PentaneLanguage::Max> LANGUAGE_CODE_3 {
	"eng", "spa", "fra", "deu", "pol", "rus", "jpn", "kor", "hin"
};

// Each language's name in itself.
inline std::array<std::string_view, PentaneLanguage::Max> LANGUAGE_NAME {
	"English", "Español", "Français", "Deutsch", "Polski", "Русский", "日本語", "한국어", "हिन्दी"
};

// Each language's name, but with all latin/cyrillic characters in their lowercase form.
inline std::array<std::string_view, PentaneLanguage::Max> LANGUAGE_NAME_LOWER {
	"english", "español", "français", "deutsch", "polski", "русский", "日本語", "한국어", "हिन्दी"
};

// Each language's name in English.
inline std::array<std::string_view, PentaneLanguage::Max> LANGUAGE_EN {
	"English", "Spanish", "French", "German", "Polish", "Russian", "Japanese", "Korean", "Hindi"
};

inline std::array<std::string_view, PentaneLanguage::Max> PENTANE_CONSOLE {
	"Pentane Console",
	"Consola Pentane",
	"Console Pentane",
	"Pentane Konsole",
	"Konsola Pentane",
	"Консоль Пентана",
	"ペンタン・コンソール",
	"펜탄 콘솔",
	"पेंटेन कंसोल"
};

inline std::array<std::string_view, PentaneLanguage::Max> PLUGIN_LOAD_SUCCESS {
	"[plugin_loader::load_plugins] Successfully loaded module: {}, Ver. {}.{}.{}, By {}",
	"[plugin_loader::load_plugins] Módulo cargado satisfactoriamente: {}, Ver. {}.{}.{}, Por {}",
	"[plugin_loader::load_plugins] Module chargé avec succès: {}, Ver. {}.{}.{}, Par {}",
	"[plugin_loader::load_plugins] Modul erfolgreich geladet : {}, Ver. {}.{}.{}, Bei {}",
	"[plugin_loader::load_plugins] Pomyślnie załadowano moduł: {}, Wer. {}.{}.{}, Zrobione przez {}",
	"[plugin_loader::load_plugins] Модуль успешно загружен: {}, Вер. {}.{}.{}, От {}.",
	"[plugin_loader::load_plugins] DLL 首尾よくロード：「{}」、バージョン： {}.{}.{}、開発者:「{}」",
	"[plugin_loader::load_plugins] DLL 성공적으로 로드: {}, 버전: {}.{}.{}, 개발자: {}",
	"[plugin_loader::load_plugins] मॉड्यूल सफलतापूर्वक लोड किया गया: {}, Ver. {}.{}.{}, By {}"
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_OLDER_DUPLICATE {
	"[plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as a newer version was already processed!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque se procesó una versión más nueva!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da eine neuere Version bereits bearbeitet wurde!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ nowsza wersja została już przetworzona!",
	"[plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как уже была обработана более новая версия!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_MISSING_MAIN {
	"[plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as its `Pentane_Main` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque faltaba su exportación `Pentane_Main`!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da der `Pentane_Main` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ brakował jego eksport `Pentane_Main`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как отсутствует экспорт `Pentane_Main`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_PENTANE_OOD {
	"[plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as it requires a newer version of Pentane!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque necesitó una versión más nueva de Pentane!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da es eine neuere Version von Pentane erfordert!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ wymaga on nowszej wersji Pentane!",
	"[plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как требуется более новая версия Pentane!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_MISSING_INFO_STRUCT_EXPORT {
	"[plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginInformation` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginInformation`!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginInformation` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginInformation`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginInformation`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_MISSING_DEPENDENCY_COUNT_EXPORT {
	"[plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginDependencyCount` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginDependencyCount`!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginDependencyCount` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginDependencyCount`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginDependencyCount`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_MISSING_DEPENDENCIES_EXPORT {
	"[plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginDependencies` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginDependencies`!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginDependencies` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginDependencies`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginDependencies`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED {
	"[plugin_loader::load_plugins] Rejected module: {} {}!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} {}!",
	"[plugin_loader::load_plugins] Module rejeté : {} {}!",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} {}!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} {}!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} {}!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_NOT_FOUND {
	"[plugin_loader::load_plugins] Rejected module: {} as it was not found in `Pentane\\Plugins`!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque no se encontró en `Pentane\\Plugins`!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da es nicht gefunden wurde in Pentane\\Plugins!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ nie został on znaleziony w Pentane\\Plugins!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как он не был найден в Pentane\\Plugins!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MODULE_REJECTED_MISSING_DEPENDENCY {
	"[plugin_loader::load_plugins] Rejected module: {} as one or more of its dependencies are missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaban una o más de sus dependencias!",
	"Unlocalized String: FRENCH",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da eine oder mehrere seiner Abhängigkeiten fehlen!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakuje jednego lub więcej jego zależności!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} из-за отсутствия одной или нескольких зависимостей!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> FILE_IN_MOD_OVERWRITES_EXISTING {
	"[fs::collect_files] File: {} in mod: {} overwrites existing mod file in: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> COLLECTED_FILE {
	"[fs::collect_files] Found file: {} in mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MOD_REJECTED_DOES_NOT_EXIST {
	"[fs::collect_files] Mod: {} was rejected as it does not exist!",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MN_FOPEN_LOADING_MODDED_FILE {
	"[fs::fopen] Loading file: {} from mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> BINK_LOADING_MODDED_FILE{
	"[fs::BinkOpen] Loading .BIK file: {} from mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MN_STREAM_LOADING_FILE{
	"[fs::BASS_StreamCreateFile] Loading stream file: {} from mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MN_SAMPLE_LOADING_FILE{
	"[fs::BASS_SampleLoad] Loading stream file: {} from mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MN_FS_FAILED_INIT {
	"[fs::init] Failed to initialize filesystem as either BASS or BinkW32 was not loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MN_DATA_DIR_TOO_LARGE {
	"[fs::init] Data directory path: {} is too large, reverting to `DataPC`!",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MN_SAVE_DIR_TOO_LARGE {
	"[fs::init] Save redirection path: {} is too large, reverting to standard save data location!",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> TVG_PAK_LOADING_MODDED_FILE {
	"[fs::PAK::OpenFile] Loading file: {} from mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> TVG_PAK_LOADING_BASE_FILE{
	"[fs::PAK::OpenFile] Loading file: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> CONFIG_PARSE_FAIL {
	"[GameConfig::read] Failed to parse `config.toml`! No plugins or mods will be loaded, all further information logged will be in English, saves will be loaded from the `Documents` folder, and game assets will be read from `DataPC`.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"[GameConfig::read] Nie udało się przeanalizować pliku `config.toml`! Nie zostaną załadowane żadne wtyczki ani mody, wszystkie dalsze informacje będą rejestrowane w języku angielskim, zapisy zostaną załadowane z folderu `Dokumenty`, a zasoby gry zostaną odczytane z `DataPC`.",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_MISSING_CONFIG {
	"[GlobalConfig::read] `config.toml` is missing the `config` section! No plugins or mods will be loaded, and the log will be in English.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"[GlobalConfig::read] W pliku `config.toml` brakuje sekcji `config`! Żadne wtyczki ani mody nie zostaną załadowane, a dziennik będzie w języku angielskim",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_MISSING_MODS {
	"[GlobalConfig::read] `config.toml` is missing the `mods` section! No mods will be loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_MISSING_PLUGINS {
	"[GlobalConfig::read] `config.toml` is missing the `plugins` section! No mods will be loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_LANG {
	"[GlobalConfig::read] `config` in `config.toml` is missing `language`! All further information logged will be in English.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"[GlobalConfig::read] W `config.toml` brakuje `language`! Wszystkie dalsze rejestrowane informacje będą w języku angielskim.",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_UNRECOGNIZED_LANG {
	"[GlobalConfig::read] Failed to recognize language set in `config`! All further information logged will be in English.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"[GlobalConfig::read] W `config.toml` brakuje `config`! Wszystkie dalsze rejestrowane informacje będą w języku angielskim.",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_MODS {
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_mods`! No mods will be loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_PLUGINS {
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_plugins`! No plugins will be loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_CONSOLE_LOGGING {
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_console_logging`! Pentane will create a console window.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_FILE_LOGGING {
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_console_window`! Pentane will create a console window.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_MODS_MISSING_ENABLED_MODS {
	"[GlobalConfig::read] `mods` in `config.toml` is missing `enabled_mods`! No mods will be loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> GLOBAL_CONFIG_PLUGINS_MISSING_ENABLED_PLUGINS {
	"[GlobalConfig::read] `plugins` in `config.toml` is missing `enabled_plugins`! No plugins will be loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MATER_NATIONAL_CONFIG_MISSING_ENABLE_SAVE_REDIRECTION {
	"[GameConfig::read] `game-config` in `config.toml` is missing `enable_save_redirection`! Saves will be loaded from the Documents folder.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MATER_NATIONAL_CONFIG_MISSING_DATA_DIRECTORY {
	"[GameConfig::read] `game-config` in `config.toml` is missing `data_directory_name`! All game assets will be read from `DataPC`.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string_view, PentaneLanguage::Max> MATER_NATIONAL_CONFIG_MISSING {
	"[GameConfig::read] `config.toml` is missing the `game-config` section! Saves will be loaded from the `Documents` folder, and game assets will be read from `DataPC`.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};