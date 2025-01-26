#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave7 : public GameWave
	{
	public:
		Wave7(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::GREEN, 0.1f, 25);
			MultipleBloons(Bloons::BloonType::YELLOW, 0.5f, 25);
			MultipleBloons(Bloons::BloonType::ZEBRA, 0.5f, 3);
		}
	};
}