#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave3 : public GameWave
	{
	public:
		Wave3(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::RED, 0.5f, 10);
			Delay(2.0f);
			MultipleBloons(Bloons::BloonType::BLUE, 0.85f, 7);
			Delay(2.0f);
			MultipleBloons(Bloons::BloonType::GREEN, 2.5f, 5);
		}
	};
}