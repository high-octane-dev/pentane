#include <cstddef>
#include <string>
#include "../logger.hpp"
#include "../plugin.hpp"
#include "../plugin_loader.hpp"
#include "../config.hpp"

struct PentaneCStringView {
	const char* const data;
	const std::size_t data_len;
};

#ifdef _M_IX86
#pragma comment(linker, "/EXPORT:Pentane_LogUTF8=_Pentane_LogUTF8")
#endif
extern "C" void Pentane_LogUTF8(PentaneCStringView* c_str) {
	if (c_str != nullptr) {
		if (c_str->data == nullptr || c_str->data_len == 0) {
			logger::log("");
		}
		else {
			std::string str(c_str->data, c_str->data_len);
			logger::log(str);
		}
	}
}

#ifdef _M_IX86
#pragma comment(linker, "/EXPORT:Pentane_IsPluginLoaded=_Pentane_IsPluginLoaded")
#endif
extern "C" int Pentane_IsPluginLoaded(PentaneUUID* uuid) {
	return plugin_loader::is_loaded(uuid) ? 1 : 0;
}

// FIXME: Is this the best way to do this?
#ifdef _M_IX86
#pragma comment(linker, "/EXPORT:Pentane_GetUserLanguage_ISO6391=_Pentane_GetUserLanguage_ISO6391")
#endif
extern "C" unsigned short Pentane_GetUserLanguage_ISO6391() {
	const auto& code = LANGUAGE_CODE[config::language()];
	return static_cast<unsigned short>(std::bit_cast<unsigned char>(code[0])) << 8 | static_cast<unsigned short>(std::bit_cast<unsigned char>(code[1]));
}

#if defined(PENTANE_GAME_TARGET_2TVG) || defined(PENTANE_GAME_TARGET_2TVGA)
#pragma comment(linker, "/EXPORT:Pentane_IsWindowedModeEnabled=_Pentane_IsWindowedModeEnabled")
extern "C" int Pentane_IsWindowedModeEnabled() {
	return config::tvg2::windowed_mode_enabled() ? 1 : 0;
}
#endif