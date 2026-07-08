#include <format>
#include "symbol_resolution.hpp"

#if defined(PENTANE_GAME_TARGET_MN)
#include "games/mn/symbols.hpp"
#elif defined(PENTANE_GAME_TARGET_2TVGA)
#include "games/2tvg/symbols_arcade.hpp"
#endif

auto crash_handler::resolve_symbol(std::uintptr_t address) ->std::optional<std::string> {
#if defined(PENTANE_GAME_TARGET_MN) || defined(PENTANE_GAME_TARGET_2TVGA)
    if (SYMBOL_MAP.empty()) {
        return std::nullopt;
    }
    auto it = std::upper_bound(SYMBOL_MAP.begin(), SYMBOL_MAP.end(), address, [](uintptr_t addr, const Symbol& sym) { return addr < sym.address; });
    if (it == SYMBOL_MAP.begin()) {
        return std::nullopt;
    }
    --it;
    return std::format("Symbol: {} + 0x{:x}, Address: 0x{:x}", it->name, address - it->address, address);
#else
    return std::nullopt;
#endif
}
