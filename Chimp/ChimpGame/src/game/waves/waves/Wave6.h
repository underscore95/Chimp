#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave6 : public GameWave
	{
	public:
		Wave6(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::PINK, 1.0f, 10);
			Delay(2.0f);
			MultipleBloons(Bloons::BloonType::BLACK, 1.5f, 10);
		}
	};
}