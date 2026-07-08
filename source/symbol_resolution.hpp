#pragma once
#include <optional>
#include <string>

namespace crash_handler {
	auto resolve_symbol(std::uintptr_t) -> std::optional<std::string>;
};