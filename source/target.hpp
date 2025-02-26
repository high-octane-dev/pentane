#pragma once

enum class PentaneTarget {
	CarsTheVideoGame = 1,
	CarsMaterNationalChampionship = 2,
	Cars2TheVideoGame = 3,
	Cars2TheVideoGameArcade = 4,
};

#if defined(PENTANE_GAME_TARGET_TVG)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::CarsTheVideoGame };
#elif defined(PENTANE_GAME_TARGET_MN)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::CarsMaterNationalChampionship };
#elif defined(PENTANE_GAME_TARGET_2TVG)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::Cars2TheVideoGame };
#elif defined(PENTANE_GAME_TARGET_2TVGA)
constexpr PentaneTarget GAME_TARGET{ PentaneTarget::Cars2TheVideoGameArcade };
#else
#error "Please define a game target!"
#endif
