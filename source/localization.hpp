#pragma once
#include <string>
#include <optional>
#include <array>

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

using FullyLocalizedString = std::array<std::string_view, PentaneLanguage::Max>;
using LocalizedString = std::array<std::optional<std::string_view>, PentaneLanguage::Max>;

namespace localization {
	constexpr inline auto support_status(PentaneLanguage language) -> bool {
		switch (language) {
		case English: return true;
		case Spanish: return true;
		case French: return false;
		case German: return false;
		case Polish: return true;
		case Russian: return true;
		case Japanese: return false;
		case Korean: return false;
		case Hindi: return false;
		default:
			return false;
		}
	}
	inline auto get_with_fallback(const LocalizedString& label, PentaneLanguage language) -> std::string_view {
		if (auto optional_format_str = label[static_cast<size_t>(language)]; optional_format_str.has_value()) {
			return optional_format_str.value();
		}
		else {
			return label[static_cast<size_t>(PentaneLanguage::English)].value();
		}
	}
};

// 2-letter latin codes for each language, as per ISO 639-1.
inline FullyLocalizedString LANGUAGE_CODE{
	"en", "es", "fr", "de", "pl", "ru", "ja", "ko", "hi"
};

// 3-letter latin codes for each language, as per ISO 639-2.
inline FullyLocalizedString LANGUAGE_CODE_3{
	"eng", "spa", "fra", "deu", "pol", "rus", "jpn", "kor", "hin"
};

// Each language's name in itself.
inline FullyLocalizedString LANGUAGE_NAME{
	"English", "Español", "Français", "Deutsch", "Polski", "Русский", "日本語", "한국어", "हिन्दी"
};

// Each language's name, but with all latin/cyrillic characters in their lowercase form.
inline FullyLocalizedString LANGUAGE_NAME_LOWER{
	"english", "español", "français", "deutsch", "polski", "русский", "日本語", "한국어", "हिन्दी"
};

// Each language's name in English.
inline FullyLocalizedString LANGUAGE_EN{
	"English", "Spanish", "French", "German", "Polish", "Russian", "Japanese", "Korean", "Hindi"
};

inline FullyLocalizedString PENTANE_CONSOLE{
	"Pentane Console",
	"Consola Pentane",
	"Console Pentane",
	"Pentane Konsole",
	"Konsola Pentane",
	"Консоль Pentane",
	"ペンタン・コンソール",
	"펜탄 콘솔",
	"पेंटेन कंसोल"
};


// This desperately needs to be fully localized, ironically enough.
inline FullyLocalizedString INCOMPLETE_LOCALIZATION{
	// This one's kinda silly and should effectively go unused.
	"English localization is not fully implemented! You may see some Pentane logger messages in English rather than your selected language.",
	"¡La localización en español no está completamente implementada! Es posible que veas algunos mensajes del registrador de Pentane en inglés en lugar de tu idioma seleccionado.",
	"French localization is NOT implemented! You will see Pentane logger messages in English instead of your selected language.", // Unsupported (Replace with "La traduction française n’est pas entièrement implémentée! Il est possible que certaines messages de logging de Pentane apparaissent en anglais au lieu de la langue sélectionnée.")
	"German localization is NOT implemented! You will see Pentane logger messages in English instead of your selected language.", // Unsupported
	"Polska lokalizacja nie jest w pełni zaimplementowana! Możesz zobaczyć niektóre komunikaty loggera Pentane w języku angielskim zamiast wybranego języka.",
	"Русская локализация не реализована полностью! Вы можете видеть некоторые сообщения логов на Английском, нежели на выбранном вами языке.",
	"Japanese localization is NOT implemented! You will see Pentane logger messages in English instead of your selected language.", // Unsupported
	"Korean localization is NOT implemented! You will see Pentane logger messages in English instead of your selected language.", // Unsupported
	"Hindi localization is NOT implemented! You will see Pentane logger messages in English instead of your selected language.", // Unsupported
};

inline LocalizedString ERROR_POPUP_TITLE{
	"Pentane Configuration Error!",
	"¡Error de configuración de Pentane!",
	std::nullopt,
	std::nullopt,
	"Błąd konfiguracji Pentane’u!",
	"Ошибка конфигурации Pentane!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TARGET_MISMATCH{
	"[DllMain] Target mismatch detected! Running game is: {}, but this Pentane build targets: {}!",
	"[DllMain] ¡Incompatibilidad de objetivo detectada! El juego en ejecución es: {}, pero esta versión de Pentane está diseñada para: {}!",
	std::nullopt,
	std::nullopt,
	"[DllMain] Wykryto niezgodność celu! Uruchomiona gra to: {}, ale ta wersja Pentane jest przeznaczona dla: {}!",
	"[DllMain] Обнаружено несовпадение целей! Игра запущена: {}, но эта сборка Pentane нацелена на: {}!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString FAILED_READ_TIMESTAMP{
	"[DllMain] Unable to detect the running game! Are you sure you're using a supported version of {}?",
	"[DllMain] ¡No se pudo detectar el juego en ejecución! ¿Estás seguro de que estás usando una versión compatible de {}?",
	std::nullopt,
	std::nullopt,
	"[DllMain] Nie można wykryć uruchomionej gry! Czy na pewno używasz obsługiwanej wersji {}?",
	"[DllMain] Невозможно обнаружить запущенную игру! Вы уверены, что используете поддерживаемую версию {}?",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline FullyLocalizedString PLUGIN_LOAD_SUCCESS{
	"[plugin_loader::load_plugins] Successfully loaded module: {}, Ver. {}.{}.{}, By {}",
	"[plugin_loader::load_plugins] Módulo cargado satisfactoriamente: {}, Ver. {}.{}.{}, Por {}",
	"[plugin_loader::load_plugins] Module chargé avec succès: {}, Ver. {}.{}.{}, Par {}",
	"[plugin_loader::load_plugins] Modul erfolgreich geladet : {}, Ver. {}.{}.{}, Bei {}",
	"[plugin_loader::load_plugins] Pomyślnie załadowano moduł: {}, Wer. {}.{}.{}, Zrobione przez {}",
	"[plugin_loader::load_plugins] Успешно загружен модуль: {}, Вер. {}.{}.{}, От {}.",
	"[plugin_loader::load_plugins] DLL 首尾よくロード：「{}」、バージョン： {}.{}.{}、開発者:「{}」",
	"[plugin_loader::load_plugins] DLL 성공적으로 로드: {}, 버전: {}.{}.{}, 개발자: {}",
	"[plugin_loader::load_plugins] मॉड्यूल सफलतापूर्वक लोड किया गया: {}, Ver. {}.{}.{}, By {}"
};

inline LocalizedString MODULE_REJECTED_OLDER_DUPLICATE{
	"[plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as a newer version was already processed!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque se procesó una versión más nueva!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da eine neuere Version bereits bearbeitet wurde!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ nowsza wersja została już przetworzona!",
	"[plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как уже была обработана более новая версия!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_MISSING_MAIN{
	"[plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as its `Pentane_Main` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque faltaba su exportación `Pentane_Main`!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da der `Pentane_Main` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ brakował jego eksport `Pentane_Main`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как отсутствует экспорт `Pentane_Main`!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_PENTANE_OOD{
	"[plugin_loader::load_plugins] Rejected module: {}, Ver. {}.{}.{}, as it requires a newer version of Pentane!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {}, Ver. {}.{}.{}, porque necesitó una versión más nueva de Pentane!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {}, Ver. {}.{}.{}, da es eine neuere Version von Pentane erfordert!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {}, Wer. {}.{}.{}, ponieważ wymaga on nowszej wersji Pentane!",
	"[plugin_loader::load_plugins] Отклонен модуль: {}, Вер. {}.{}.{}, так как требуется более новая версия Pentane!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_MISSING_INFO_STRUCT_EXPORT{
	"[plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginInformation` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginInformation`!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginInformation` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginInformation`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginInformation`!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_MISSING_DEPENDENCY_COUNT_EXPORT{
	"[plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginDependencyCount` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginDependencyCount`!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginDependencyCount` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginDependencyCount`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginDependencyCount`!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_MISSING_DEPENDENCIES_EXPORT{
	"[plugin_loader::load_plugins] Rejected module: {} as its `Pentane_PluginDependencies` export was missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaba su exportación `Pentane_PluginDependencies`!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da der `Pentane_PluginDependencies` export fehlte!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakował jego eksport `Pentane_PluginDependencies`!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как отсутствует экспорт `Pentane_PluginDependencies`!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED{
	"[plugin_loader::load_plugins] Rejected module: {} {}!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} {}!",
	"[plugin_loader::load_plugins] Module rejeté : {} {}!",
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} {}!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} {}!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} {}!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_NOT_FOUND{
	"[plugin_loader::load_plugins] Rejected module: {} as it was not found in `Pentane\\Plugins`!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque no se encontró en `Pentane\\Plugins`!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da es nicht gefunden wurde in Pentane\\Plugins!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ nie został on znaleziony w Pentane\\Plugins!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} так как он не был найден в Pentane\\Plugins!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MODULE_REJECTED_MISSING_DEPENDENCY{
	"[plugin_loader::load_plugins] Rejected module: {} as one or more of its dependencies are missing!",
	"[plugin_loader::load_plugins] ¡Módulo rechazado: {} porque faltaban una o más de sus dependencias!",
	std::nullopt,
	"[plugin_loader::load_plugins] Modul Abgelehnt: {} da eine oder mehrere seiner Abhängigkeiten fehlen!",
	"[plugin_loader::load_plugins] Odrzucono moduł: {} ponieważ brakuje jednego lub więcej jego zależności!",
	"[plugin_loader::load_plugins] Отклонен модуль: {} из-за отсутствия одной или нескольких зависимостей!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString FILE_IN_MOD_OVERWRITES_EXISTING{
	"[fs::collect_files] File: {} in mod: {} overwrites existing mod file in: {}",
	"[fs::collect_files] El archivo: {} en el mod: {} sobrescribe un archivo de mod existente en: {}",
	std::nullopt,
	std::nullopt,
	"[fs::collect_files] Plik: {} w modzie: {} nadpisuje istniejący plik moda w: {}",
	"[fs::collect_files] Файл: {} в моде: {} перезаписывает существующий файл мода в: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString COLLECTED_FILE{
	"[fs::collect_files] Found file: {} in mod: {}",
	"[fs::collect_files] Archivo encontrado: {} en el mod: {}",
	std::nullopt,
	std::nullopt,
	"[fs::collect_files] Znaleziono plik: {} w modzie: {}",
	"[fs::collect_files] Найден файл: {} в моде: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MOD_REJECTED_DOES_NOT_EXIST{
	"[fs::collect_files] Mod: {} was rejected as it does not exist!",
	"[fs::collect_files] ¡El mod: {} fue rechazado porque no existe!",
	std::nullopt,
	std::nullopt,
	"[fs::collect_files] Mod: {} został odrzucony, ponieważ nie istnieje!",
	"[fs::collect_files] Мод: {} отклонён так как его не существует!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_FOPEN_LOADING_MODDED_FILE{
	"[fs::fopen] Loading file: {} from mod: {}",
	"[fs::fopen] Cargando archivo: {} desde el mod: {}",
	std::nullopt,
	std::nullopt,
	"[fs::fopen] Wczytywanie pliku: {} z moda: {}",
	"[fs::fopen] Загрузка файла: {} из мода: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString BINK_LOADING_MODDED_FILE{
	"[fs::BinkOpen] Loading .BIK file: {} from mod: {}",
	"[fs::BinkOpen] Cargando archivo .BIK: {} desde el mod: {}",
	std::nullopt,
	std::nullopt,
	"[fs::BinkOpen] Wczytywanie pliku .BIK: {} z moda: {}",
	"[fs::BinkOpen] Загрузка .BIK файла: {} из мода: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_STREAM_LOADING_FILE{
	"[fs::BASS_StreamCreateFile] Loading stream file: {} from mod: {}",
	"[fs::BASS_StreamCreateFile] Cargando archivo de stream: {} desde el mod: {}",
	std::nullopt,
	std::nullopt,
	"[fs::BASS_StreamCreateFile] Wczytywanie pliku strumieniowego: {} z moda: {}",
	"[fs::BASS_StreamCreateFile] Загрука файла потока: {} из мода: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_SAMPLE_LOADING_FILE{
	"[fs::BASS_SampleLoad] Loading stream file: {} from mod: {}",
	"[fs::BASS_SampleLoad] Cargando archivo de muestra: {} desde el mod: {}",
	std::nullopt,
	std::nullopt,
	"[fs::BASS_SampleLoad] Wczytywanie pliku stream: {} z moda: {}",
	"[fs::BASS_SampleLoad] Загрука файла потока: {} из мода: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_FS_FAILED_INIT{
	"[fs::init] Failed to initialize filesystem as either BASS or BinkW32 was not loaded.",
	"[fs::init] Error al inicializar el sistema de archivos porque no se cargó BASS ni BinkW32.",
	std::nullopt,
	std::nullopt,
	"[fs::init] Nie udało się zainicjować systemu plików, ponieważ BASS lub BinkW32 nie został załadowany.",
	"[fs::init] Не удалось инициализировать файловую систему, поскольку ни BASS, ни BinkW32 не были загружены.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_DATA_DIR_TOO_LARGE{
	"[fs::init] Data directory path: {} is too large, reverting to `DataPC`!",
	"[fs::init] La ruta del directorio de datos: {} es demasiado larga, se volverá a `DataPC`!",
	std::nullopt,
	std::nullopt,
	"[fs::init] Ścieżka do katalogu danych: {} jest zbyt duża, powrót do `DataPC`!",
	"[fs::init] Путь к каталогу данных: {} слишком велик, возвращаемся к `DataPC`!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_SAVE_DIR_TOO_LARGE{
	"[fs::init] Save redirection path: {} is too large, reverting to standard save data location!",
	"[fs::init] La ruta de redirección de guardado: {} es demasiado larga, se revertirá a la ubicación estándar de datos guardados!",
	std::nullopt,
	std::nullopt,
	"[fs::init] Ścieżka przekierowania zapisu: {} jest zbyt duża, powrót do standardowej lokalizacji zapisu danych!",
	"[fs::init] Путь перенаправления сохранения: {} слишком велик, возвращаемся к стандартному расположению данных сохранения!",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG_PAK_LOADING_MODDED_FILE{
	"[fs::PAK::OpenFile] Loading file: {} from mod: {}",
	"[fs::PAK::OpenFile] Cargando archivo: {} desde el mod: {}",
	std::nullopt,
	std::nullopt,
	"[fs::PAK::OpenFile] Wczytywanie pliku: {} z moda: {}",
	"[fs::PAK::OpenFile] Загрузка файла: {} из мода: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG_PAK_LOADING_BASE_FILE{
	"[fs::PAK::OpenFile] Loading file: {}",
	"[fs::PAK::OpenFile] Cargando archivo: {}",
	std::nullopt,
	std::nullopt,
	"[fs::PAK::OpenFile] Wczytywanie pliku: {}",
	"[fs::PAK::OpenFile] Загрузка файла: {}",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG_CONFIG_PARSE_FAIL{
	"[GameConfig::read] Failed to parse `config.toml`! No plugins or mods will be loaded, all further information logged will be in English, and saves will be loaded from the `Documents` folder.",
	"[GameConfig::read] ¡Error al analizar `config.toml`! No se cargarán plugins ni mods, toda la información registrada a partir de ahora estará en inglés y las partidas guardadas se cargarán desde la carpeta `Documentos`.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] Nie udało się przeanalizować pliku `config.toml`! Nie zostaną załadowane żadne wtyczki ani mody, wszystkie dalsze informacje będą rejestrowane w języku angielskim, a zapisy zostaną załadowane z folderu `Dokumenty`.",
	"[GameConfig::read] Не удалось разобрать `config.toml`! Никакие плагины или моды загружаться не будут, вся дальнейшая информация будет записываться на английском языке, а сохранения будут загружаться из папки `Документы`.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MN_CONFIG_PARSE_FAIL{
	"[GameConfig::read] Failed to parse `config.toml`! No plugins or mods will be loaded, all further information logged will be in English, saves will be loaded from the `Documents` folder, and game assets will be read from `DataPC`.",
	"[GameConfig::read] ¡Error al analizar `config.toml`! No se cargarán plugins ni mods, toda la información registrada a partir de ahora estará en inglés, las partidas guardadas se cargarán desde la carpeta `Documentos` y los recursos del juego se leerán desde `DataPC`.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] Nie udało się przeanalizować pliku `config.toml`! Nie zostaną załadowane żadne wtyczki ani mody, wszystkie dalsze informacje będą rejestrowane w języku angielskim, zapisy zostaną załadowane z folderu `Dokumenty`, a zasoby gry zostaną odczytane z `DataPC`.",
	"[GameConfig::read] Не удалось разобрать `config.toml`! Никакие плагины или моды загружаться не будут, вся дальнейшая информация будет записываться на английском языке, сохранения будут загружаться из папки `Документы`, а игровые активы будут считываться из `DataPC`.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG2_CONFIG_PARSE_FAIL{
	"[GameConfig::read] Failed to parse `config.toml`! No plugins will be loaded, all further information logged will be in English, and the game will run in exclusive fullscreen mode.",
	"[GameConfig::read] ¡Error al analizar `config.toml`! No se cargarán plugins, toda la información registrada a partir de ahora estará en inglés y el juego se ejecutará en modo de pantalla completa exclusiva.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] Не удалось разобрать `config.toml`! Никакие плагины загружаться не будут, вся дальнейшая информация будет записываться на английском языке, а игра будет работать исключительно в полноэкранном режиме.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_MISSING_CONFIG{
	"[GlobalConfig::read] `config.toml` is missing the `config` section! No plugins or mods will be loaded, and the log will be in English.",
	"[GlobalConfig::read] ¡Falta la sección `config` en `config.toml`! No se cargarán plugins ni mods, y el registro estará en inglés.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] W pliku `config.toml` brakuje sekcji `config`! Żadne wtyczki ani mody nie zostaną załadowane, a dziennik będzie w języku angielskim",
	"[GlobalConfig::read] В файле `config.toml` отсутствует раздел `config`! Никакие плагины или моды не будут загружены, а журнал будет на английском языке.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_MISSING_MODS{
	"[GlobalConfig::read] `config.toml` is missing the `mods` section! No mods will be loaded.",
	"[GlobalConfig::read] ¡Falta la sección `mods` en `config.toml`! No se cargarán mods.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] Plik `config.toml` nie zawiera sekcji `mods` ! Żadne mody nie zostaną załadowane.",
	"[GlobalConfig::read] В файле `config.toml` отсутствует раздел `mods`! Ни один мод не будет загружен.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_MISSING_PLUGINS{
	"[GlobalConfig::read] `config.toml` is missing the `plugins` section! No plugins will be loaded.",
	"[GlobalConfig::read] ¡Falta la sección plugins en `config.toml`! No se cargarán `plugins`.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] Plik `config.toml` nie zawiera sekcji `plugins` ! Żadne wtyczki nie zostaną załadowane.",
	"[GlobalConfig::read] В файле `config.toml` отсутствует раздел `plugins`! Никакие плагины не будут загружены.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_CONFIG_MISSING_LANG{
	"[GlobalConfig::read] `config` in `config.toml` is missing `language`! All further information logged will be in English.",
	"[GlobalConfig::read] ¡Falta `language` en la sección `config` de `config.toml`! Toda la información registrada a partir de ahora estará en inglés.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] W `config.toml` brakuje `language`! Wszystkie dalsze rejestrowane informacje będą w języku angielskim.",
	"[GlobalConfig::read] `config` в `config.toml` не хватает `language`! Вся дальнейшая информация будет записываться на английском языке.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_CONFIG_UNRECOGNIZED_LANG{
	"[GlobalConfig::read] Failed to recognize language set in `config`! All further information logged will be in English.",
	"[GlobalConfig::read] ¡No se pudo reconocer el idioma establecido en la sección `config`! Toda la información registrada a partir de ahora estará en inglés.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] W `config.toml` brakuje `config`! Wszystkie dalsze rejestrowane informacje będą w języku angielskim.",
	"[GlobalConfig::read] Не удалось распознать язык, заданный в `config`! Вся дальнейшая информация будет записываться на английском языке.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_MODS{
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_mods`! No mods will be loaded.",
	"[GlobalConfig::read] ¡Falta `enable_mods` en la sección `config` de `config.toml`! No se cargarán mods.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] `config` в `config.toml` не хватает `enable_mods`! Ни один мод не будет загружен.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_PLUGINS{
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_plugins`! No plugins will be loaded.",
	"[GlobalConfig::read] ¡Falta `enable_plugins` en la sección `config` de `config.toml`! No se cargarán plugins.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] Sekcja `config` w pliku `config.toml` nie zawiera `enable_mods` ! Żadne mody nie zostaną załadowane.",
	"[GlobalConfig::read] `config` в `config.toml` не хватает `enable_plugins`! Никакие плагины не будут загружены.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_CONSOLE_LOGGING{
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_console_logging`! Pentane will create a console window.",
	"[GlobalConfig::read] ¡Falta `enable_console_logging` en la sección `config` de `config.toml`! Pentane creará una ventana de consola.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] Sekcja `config` w pliku `config.toml` nie zawiera `enable_console_logging`! Pentane utworzy okno konsoli.",
	"[GlobalConfig::read] `config` в `config.toml` не хватает `enable_console_logging`! Pentane создаст консольное окно.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_CONFIG_MISSING_ENABLE_FILE_LOGGING{
	"[GlobalConfig::read] `config` in `config.toml` is missing `enable_file_logging`! Pentane will write its log to `pentane.log`.",
	"[GlobalConfig::read] ¡Falta `enable_file_logging` en la sección `config` de `config.toml`! Pentane escribirá su registro en pentane.log.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] `config` в `config.toml` не хватает `enable_file_logging`! Pentane будет записывать свой журнал в `pentane.log`.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_MODS_MISSING_ENABLED_MODS{
	"[GlobalConfig::read] `mods` in `config.toml` is missing `enabled_mods`! No mods will be loaded.",
	"[GlobalConfig::read] ¡Falta `enabled_mods` en la sección `mods` de `config.toml`! No se cargarán mods.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] Sekcja mods w pliku config.toml nie zawiera enabled_mods! Żadne mody nie zostaną załadowane.",
	"[GlobalConfig::read] `mods` в `config.toml` не хватает `enabled_mods`! Ни один мод не будет загружен.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString GLOBAL_CONFIG_PLUGINS_MISSING_ENABLED_PLUGINS{
	"[GlobalConfig::read] `plugins` in `config.toml` is missing `enabled_plugins`! No plugins will be loaded.",
	"[GlobalConfig::read] ¡Falta `enabled_plugins` en la sección `plugins` de `config.toml`! No se cargarán plugins.",
	std::nullopt,
	std::nullopt,
	"[GlobalConfig::read] Sekcja `plugins` w pliku `config.toml` nie zawiera `enabled_plugins`! Żadne wtyczki nie zostaną załadowane.",
	"[GlobalConfig::read] `plugins` в `config.toml` отсутствует `enabled_plugins`! Никакие плагины не будут загружены.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MATER_NATIONAL_CONFIG_MISSING_ENABLE_SAVE_REDIRECTION{
	"[GameConfig::read] `game-config` in `config.toml` is missing `enable_save_redirection`! Saves will be loaded from the Documents folder.",
	"[GameConfig::read] ¡Falta `enable_save_redirection` en la sección game-config de `config.toml`! Las partidas guardadas se cargarán desde la carpeta Documentos.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] sekcja `game-config` w pliku `config.toml` nie zawiera `enable_save_redirection`! Zapisy będą ładowane z folderu Dokumenty.",
	"[GameConfig::read] `game-config` в `config.toml` не хватает `enable_save_redirection`! Сохранения будут загружаться из папки Документов.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MATER_NATIONAL_CONFIG_MISSING_DATA_DIRECTORY{
	"[GameConfig::read] `game-config` in `config.toml` is missing `data_directory_name`! All game assets will be read from `DataPC`.",
	"[GameConfig::read] ¡Falta `data_directory_name` en la sección game-config de `config.toml`! Todos los recursos del juego se leerán desde `DataPC`.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] sekcja `game-config` w pliku `config.toml` nie zawiera `data_directory_name`! Wszystkie zasoby gry będą odczytywane z `DataPC`.",
	"[GameConfig::read] `game-config` в `config.toml` не хватает `data_directory_name`! Все игровые ассеты будут считаны из `DataPC`.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString MATER_NATIONAL_CONFIG_MISSING{
	"[GameConfig::read] `config.toml` is missing the `game-config` section! Saves will be loaded from the `Documents` folder, and game assets will be read from `DataPC`.",
	"[GameConfig::read] ¡Falta la sección game-config en config.toml! Las partidas guardadas se cargarán desde la carpeta `Documentos` y los recursos del juego se leerán desde `DataPC`.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] plik `config.toml` nie zawiera sekcji `game-config`! Zapisy będą ładowane z folderu `Documents`, a zasoby gry będą odczytywane z `DataPC`.",
	"[GameConfig::read] В файле `config.toml` отсутствует раздел `game-config`! Сохранения будут загружаться из папки `Документы`, а игровые активы будут считываться из `DataPC`.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG2_CONFIG_MISSING_ENABLE_WINDOWED_MODE{
	"[GameConfig::read] `game-config` in `config.toml` is missing `enable_windowed_mode`! The game will create an exclusive fullscreen window.",
	"[GameConfig::read] ¡Falta `enable_windowed_mode` en la sección `game-config` de `config.toml`! El juego creará una ventana en modo pantalla completa exclusiva.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] W sekcji game-config w pliku config.toml brakuje opcji enable_windowed_mode! Gra uruchomi się w trybie pełnoekranowym.",
	"[GameConfig::read] `game-config` в `config.toml` не хватает `enable_windowed_mode`! Игра создаст эксклюзивное полноэкранное окно.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG2A_CONFIG_MISSING_WINDOW_WIDTH{
	"[GameConfig::read] `game-config` in `config.toml` is missing `windowed_mode_width`! The game window will try to match 1280x720.",
	"[GameConfig::read] ¡Falta `windowed_mode_width` en la sección game-config de `config.toml`! La ventana del juego intentará ajustarse a 1280x720.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] W sekcji game-config w pliku config.toml brakuje opcji `windowed_mode_width`! Okno gry spróbuje dopasować się do rozdzielczości 1280x720.",
	"[GameConfig::read] `game-config` в `config.toml` не хватает `windowed_mode_width`! Окно игры будет пытаться установить размер 1280x720.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG2A_CONFIG_MISSING_WINDOW_HEIGHT{
	"[GameConfig::read] `game-config` in `config.toml` is missing `windowed_mode_height`! The game window will try to match 1280x720.",
	"[GameConfig::read] ¡Falta `windowed_mode_height` en la sección `game-config` de `config.toml`! La ventana del juego intentará ajustarse a 1280x720.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] W sekcji game-config w pliku config.toml brakuje opcji `windowed_mode_height`! Okno gry spróbuje dopasować się do rozdzielczości 1280x720.",
	"[GameConfig::read] `game-config` в `config.toml` не хватает `windowed_mode_height`! Окно игры будет пытаться установить размер 1280x720.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};

inline LocalizedString TVG2_CONFIG_MISSING{
	"[GameConfig::read] `config.toml` is missing the `game-config` section! The game will run in exclusive fullscreen mode.",
	"[GameConfig::read] ¡Falta la sección `game-config` en `config.toml`! El juego se ejecutará en modo pantalla completa exclusiva.",
	std::nullopt,
	std::nullopt,
	"[GameConfig::read] W pliku config.toml brakuje sekcji game-config! Gra uruchomi się w trybie pełnoekranowym na wyłączność.,"
	"[GameConfig::read] В файле `config.toml` отсутствует раздел `game-config`! Игра будет работать исключительно в полноэкранном режиме.",
	std::nullopt,
	std::nullopt,
	std::nullopt,
};
