#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave5 : public GameWave
	{
	public:
		Wave5(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::RED, 0.1f, 50);
			MultipleBloons(Bloons::BloonType::YELLOW, 1.0f, 10);
		}
	};
}