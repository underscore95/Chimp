#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave9 : public GameWave
	{
	public:
		Wave9(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::PINK, 0.2f, 10);
			MultipleBloons(Bloons::BloonType::BLACK, 0.5f, 10);
			MultipleBloons(Bloons::BloonType::ZEBRA, 2.5f, 10);
		}
	};
}