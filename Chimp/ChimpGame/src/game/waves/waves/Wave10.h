#pragma once

#include "game/waves/GameWave.h"

namespace Waves
{
	class Wave10 : public GameWave
	{
	public:
		Wave10(BloonManager& bloonManager)
			: GameWave(bloonManager) {
			MultipleBloons(Bloons::BloonType::RED, 0.05f, 500);
			MultipleBloons(Bloons::BloonType::RAINBOW, 3.0f, 5);
		}
	};
}