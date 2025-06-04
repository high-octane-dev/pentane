#pragma once
#include <format>

enum class PentaneTarget {
	Invalid = 0,
	CarsTheVideoGame = 1,
	CarsMaterNationalChampionship = 2,
	Cars2TheVideoGame = 3,
	Cars2TheVideoGameArcade = 4,
	Cars3DrivenToWin = 5,
};


template <>
struct std::formatter<PentaneTarget> : std::formatter<std::string_view> {
    auto format(PentaneTarget target, format_context& ctx) const {
        std::string_view name;
        switch (target) {
        case PentaneTarget::Invalid: name = "Invalid"; break;
        case PentaneTarget::CarsTheVideoGame: name = "Cars: The Video Game"; break;
        case PentaneTarget::CarsMaterNationalChampionship: name = "Cars: Mater-National Championship"; break;
        case PentaneTarget::Cars2TheVideoGame: name = "Cars 2: The Video Game"; break;
        case PentaneTarget::Cars2TheVideoGameArcade: name = "Cars 2: Arcade"; break;
        case PentaneTarget::Cars3DrivenToWin: name = "Cars 3: Driven To Win"; break;
        default: name = "Invalid"; break;
        }
        return std::formatter<std::string_view>::format(name, ctx);
    }
};

#if defined(PENTANE_GAME_TARGET_TVG)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::CarsTheVideoGame };
#elif defined(PENTANE_GAME_TARGET_MN)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::CarsMaterNationalChampionship };
#elif defined(PENTANE_GAME_TARGET_2TVG)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::Cars2TheVideoGame };
#elif defined(PENTANE_GAME_TARGET_2TVGA)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::Cars2TheVideoGameArcade };
#elif defined(PENTANE_GAME_TARGET_3DTW)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::Cars3DrivenToWin };
#else
#error "Please define a game target!"
#endif
