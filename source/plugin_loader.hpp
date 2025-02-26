#pragma once
#include <filesystem>
#include "plugin.hpp"

namespace plugin_loader {
	auto load_all(const std::filesystem::path&) -> void;
	auto is_loaded(PentaneUUID* uuid) -> bool;
};
