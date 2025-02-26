#pragma once
#include <string>
#include <array>

#define LOG_LOCALIZED_STRING(LABEL, ...) { std::string value = std::vformat(LABEL[config::language()], std::make_format_args(__VA_ARGS__)); logger::log(value); }

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

// 2-letter latin codes for each language, as per ISO 639-1.
inline std::array<std::string, PentaneLanguage::Max> LANGUAGE_CODE {
	"en", "es", "fr", "de", "pl", "ru", "ja", "ko", "hi"
};

// 3-letter latin codes for each language, as per ISO 639-2.
inline std::array<std::string, PentaneLanguage::Max> LANGUAGE_CODE_3 {
	"eng", "spa", "fra", "deu", "pol", "rus", "jpn", "kor", "hin"
};

// Each language's name in itself.
inline std::array<std::string, PentaneLanguage::Max> LANGUAGE_NAME {
	"English", "Español", "Français", "Deutsch", "Polski", "Русский", "日本語", "한국어", "हिन्दी"
};

// Each language's name, but with all latin/cyrillic characters in their lowercase form.
inline std::array<std::string, PentaneLanguage::Max> LANGUAGE_NAME_LOWER {
	"english", "español", "français", "deutsch", "polski", "русский", "日本語", "한국어", "हिन्दी"
};

// Each language's name in English.
inline std::array<std::string, PentaneLanguage::Max> LANGUAGE_EN {
	"English", "Spanish", "French", "German", "Polish", "Russian", "Japanese", "Korean", "Hindi"
};

inline std::array<std::string, PentaneLanguage::Max> PENTANE_CONSOLE {
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

inline std::array<std::string, PentaneLanguage::Max> PLUGIN_LOAD_SUCCESS {
	"[pentane::plugin_loader::load_plugins] Successfully loaded module: {}, Ver. {}.{}.{}, By {}",
	"[pentane::plugin_loader::load_plugins] Módulo cargado satisfactoriamente: {}, Ver. {}.{}.{}, Por {}",
	"[pentane::plugin_loader::load_plugins] Module chargé avec succès: {}, Ver. {}.{}.{}, Par {}",
	"[pentane::plugin_loader::load_plugins] Modul erfolgreich geladet : {}, Ver. {}.{}.{}, Bei {}",
	"[pentane::plugin_loader::load_plugins] Pomyślnie załadowano moduł: {}, Wer. {}.{}.{}, Według {}",
	"[pentane::plugin_loader::load_plugins] Модуль успешно загружен: {}, Вер. {}.{}.{}, От {}.",
	"[pentane::plugin_loader::load_plugins] DLL 首尾よくロード：「{}」、バージョン： {}.{}.{}、開発者:「{}」",
	"[pentane::plugin_loader::load_plugins] DLL 성공적으로 로드: {}, 버전: {}.{}.{}, 개발자: {}",
	"[pentane::plugin_loader::load_plugins] मॉड्यूल सफलतापूर्वक लोड किया गया: {}, Ver. {}.{}.{}, By {}"
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_OLDER_DUPLICATE {
	"[pentane::plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as a newer version was already processed!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque se procesó una versión más nueva!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da eine neuere Version bereits bearbeitet wurde!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ nowsza wersja została już przetworzona!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как уже была обработана более новая версия!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_MISSING_MAIN {
	"[pentane::plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as its `Pentane_Main` export was missing!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque faltaba su exportación `Pentane_Main`!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da der `Pentane_Main` export fehlte!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ brakował jego eksport `Pentane_Main`!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как отсутствует экспорт `Pentane_Main`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_PENTANE_OOD {
	"[pentane::plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as it requires a newer version of Pentane!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque necesitó una versión más nueva de Pentane!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da es eine neuere Version von Pentane erfordert!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ wymaga on nowszej wersji Pentane!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как требуется более новая версия Pentane!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_MISSING_INFO_STRUCT_EXPORT {
	"[pentane::plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginInformation` export was missing!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginInformation`!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginInformation` export fehlte!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginInformation`!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginInformation`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_MISSING_DEPENDENCY_COUNT_EXPORT {
	"[pentane::plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginDependencyCount` export was missing!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginDependencyCount`!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginDependencyCount` export fehlte!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginDependencyCount`!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginDependencyCount`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_MISSING_DEPENDENCIES_EXPORT {
	"[pentane::plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginDependencies` export was missing!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginDependencies`!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginDependencies` export fehlte!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginDependencies`!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginDependencies`!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED {
	"[pentane::plugin_loader::load_plugins] Rejected module: {}!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {}!",
	"[pentane::plugin_loader::load_plugins] Module rejeté : {} !",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {}!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {}!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {}!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_NOT_FOUND {
	"[pentane::plugin_loader::load_plugins] Rejected module: {} as it was not found in `LocalStorage\\Plugins`!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {} porque no se encontró en `LocalStorage\\Plugins`!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {} da es nicht gefunden wurde in LocalStorage\\Plugins!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ nie został on znaleziony w LocalStorage\\Plugins!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {} так как он не был найден в LocalStorage\\Plugins!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MODULE_REJECTED_MISSING_DEPENDENCY {
	"[pentane::plugin_loader::load_plugins] Rejected module: {} as one or more of its dependencies are missing!",
	"[pentane::plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaban una o más de sus dependencias!",
	"Unlocalized String: FRENCH",
	"[pentane::plugin_loader::load_plugins] Modul Abgelehnt: {} da eine oder mehrere seiner Abhängigkeiten fehlen!",
	"[pentane::plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakuje jednego lub więcej jego zależności!",
	"[pentane::plugin_loader::load_plugins] Отклонен модуль: {} из-за отсутствия одной или нескольких зависимостей!",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> FILE_IN_MOD_OVERWRITES_EXISTING {
	"[pentane::mn::fs::collect_files] File: {} in mod: {} overwrites existing mod file in: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> COLLECTED_FILE {
	"[pentane::mn::fs::collect_files] Found file: {} in mod: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MOD_REJECTED_DOES_NOT_EXIST {
	"[pentane::mn::fs::collect_files] Mod: {} was rejected as it does not exist!",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> FOPEN_LOADING_FILE {
	"[pentane::mn::fs::fopen] Loading file: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> BINK_LOADING_FILE{
	"[pentane::mn::fs::BinkOpen] Loading .BIK file: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> STREAM_LOADING_FILE{
	"[pentane::mn::fs::BASS_StreamCreateFile] Loading stream file: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> SAMPLE_LOADING_FILE{
	"[pentane::mn::fs::BASS_SampleLoad] Loading stream file: {}",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> FS_FAILED_INIT {
	"[pentane::mn::fs::init] Failed to initialize filesystem as either BASS or BinkW32 was not loaded.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> DATA_DIR_TOO_LARGE {
	"[pentane::mn::fs::init] Data directory path: {} is too large, reverting to `DataPC`!",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> SAVE_DIR_TOO_LARGE {
	"[pentane::mn::fs::init] Save redirection path: {} is too large, reverting to standard save data location!",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> CONFIG_PARSE_FAIL {
	"[MaterNationalConfig::read] Failed to parse `config.toml`! No plugins or mods will be loaded, all further information logged will be in English, saves will be loaded from the `Documents` folder, and game assets will be read from `DataPC`.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_MISSING_CONFIG {
	"[GlobalConfig::read] `config.toml` is missing the `config` section! No plugins or mods will be loaded, and the log will be in English.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_MISSING_MODS {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_MISSING_PLUGINS {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_LANG {
	"[GlobalConfig::read] `config` in `config.toml` is missing `language`! All further information logged will be in English.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_UNRECOGNIZED_LANG {
	"[GlobalConfig::read] Failed to recognize language set in `config`! All further information logged will be in English.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_MODS {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_PLUGINS {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_CONSOLE_LOGGING {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_FILE_LOGGING {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_MODS_MISSING_ENABLED_MODS {
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

inline std::array<std::string, PentaneLanguage::Max> GLOBAL_CONFIG_PLUGINS_MISSING_ENABLED_PLUGINS {
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

inline std::array<std::string, PentaneLanguage::Max> MATER_NATIONAL_CONFIG_MISSING_ENABLE_SAVE_REDIRECTION {
	"[MaterNationalConfig::read] `game-config` in `config.toml` is missing `enable_save_redirection`! Saves will be loaded from the Documents folder.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MATER_NATIONAL_CONFIG_MISSING_DATA_DIRECTORY {
	"[MaterNationalConfig::read] `game-config` in `config.toml` is missing `data_directory_name`! All game assets will be read from `DataPC`.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};

inline std::array<std::string, PentaneLanguage::Max> MATER_NATIONAL_CONFIG_MISSING {
	"[MaterNationalConfig::read] `config.toml` is missing the `game-config` section! Saves will be loaded from the `Documents` folder, and game assets will be read from `DataPC`.",
	"Unlocalized String: SPANISH",
	"Unlocalized String: FRENCH",
	"Unlocalized String: GERMAN",
	"Unlocalized String: POLISH",
	"Unlocalized String: RUSSIAN",
	"Unlocalized String: JAPANESE",
	"Unlocalized String: KOREAN",
	"Unlocalized String: HINDI",
};