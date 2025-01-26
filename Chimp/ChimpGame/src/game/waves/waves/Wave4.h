#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave4 : public GameWave
	{
	public:
		Wave4(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::GREEN, 0.75f, 15);
			MultipleBloons(Bloons::BloonType::RED, 0.1f, 50);
			MultipleBloons(Bloons::BloonType::BLUE, 0.5f, 20);
			MultipleBloons(Bloons::BloonType::BLACK, 1.0f, 5);
		}
	};
}