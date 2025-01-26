#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave8 : public GameWave
	{
	public:
		Wave8(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::ZEBRA, 2.0f, 5);
			MultipleBloons(Bloons::BloonType::YELLOW, 0.5f, 25);
			SingleBloon(Bloons::BloonType::RAINBOW);
			MultipleBloons(Bloons::BloonType::PINK, 0.75f, 25);
			SingleBloon(Bloons::BloonType::RAINBOW);
		}
	};
}