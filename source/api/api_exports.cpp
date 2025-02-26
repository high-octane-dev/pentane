#include <cstddef>
#include <string>
#include "../logger.hpp"
#include "../plugin.hpp"
#include "../plugin_loader.hpp"

struct PentaneCStringView {
	const char* const data;
	const std::size_t data_len;
};

#pragma comment(linker, "/EXPORT:Pentane_LogUTF8=_Pentane_LogUTF8")
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

#pragma comment(linker, "/EXPORT:Pentane_IsPluginLoaded=_Pentane_IsPluginLoaded")
extern "C" int Pentane_IsPluginLoaded(PentaneUUID* uuid) {
	return plugin_loader::is_loaded(uuid) ? 1 : 0;
}