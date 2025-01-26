#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave2 : public GameWave
	{
	public:
		Wave2(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::BLUE, 0.25f, 5);
			MultipleBloons(Bloons::BloonType::RED, 0.5f, 10);
			MultipleBloons(Bloons::BloonType::BLUE, 0.5f, 5);
		}
	};
}