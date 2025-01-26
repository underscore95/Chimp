#pragma once

#include "stdafx.h"
#include "game/Entities.h"
#include "game/bloons/BloonManager.h"

class GameWave : public Chimp::Wave
{
protected:
	GameWave(BloonManager& bloonManager);
public:
	virtual ~GameWave() = default;

	struct MultipleBloonsDesc {
		Bloons::BloonType bloonType = Bloons::BloonType::RED;
		float intervalSeconds = 0.25f;
		unsigned int numBloons = 1;
	};

protected:
	void SingleBloon(Bloons::BloonType bloonType);
	void MultipleBloons(Bloons::BloonType bloonType, float intervalSeconds, unsigned int numBloons);
	void MultipleBloons(MultipleBloonsDesc bloons);

private:
	BloonManager& m_BloonManager;
};