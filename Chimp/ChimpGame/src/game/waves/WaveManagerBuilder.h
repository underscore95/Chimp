#pragma once

#include "game/bloons/BloonManager.h"

#include "waves/Wave1.h"
#include "waves/Wave2.h"
#include "waves/Wave3.h"
#include "waves/Wave4.h"
#include "waves/Wave5.h"
#include "waves/Wave6.h"
#include "waves/Wave7.h"
#include "waves/Wave8.h"
#include "waves/Wave9.h"
#include "waves/Wave10.h"

class WaveManagerBuilder
{
private:
	WaveManagerBuilder() = delete;

public:
	static std::unique_ptr<Chimp::WaveManager> Build(
		Chimp::Engine& engine,
		BloonManager& bloonManager)
	{
		return Chimp::WaveManager::Builder(engine)
			.AddWave(std::make_unique<Waves::Wave1>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave2>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave3>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave4>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave5>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave6>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave7>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave8>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave9>(bloonManager))
			.AddWave(std::make_unique<Waves::Wave10>(bloonManager))
			.Build();
	}
};