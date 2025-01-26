#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave1 : public GameWave
	{
	public:
		Wave1(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			Delay(8);
			MultipleBloons(Bloons::BloonType::RED, 1.0f, 5);
			Delay(5);
			MultipleBloons(Bloons::BloonType::RED, 1.0f, 20);
		}
	};
}