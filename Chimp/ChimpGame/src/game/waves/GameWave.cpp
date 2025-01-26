#include "GameWave.h"

GameWave::GameWave(BloonManager& bloonManager)
	: m_BloonManager(bloonManager)
{
}

void GameWave::SingleBloon(Bloons::BloonType bloonType)
{
	RunFunction([this, bloonType](Chimp::Engine& engine) {
		m_BloonManager.SpawnBloon(bloonType);
		});
}

void GameWave::MultipleBloons(Bloons::BloonType bloonType, float intervalSeconds, unsigned int numBloons)
{
	RunRepeatingFunction([this, bloonType](Chimp::Engine& engine) {
		m_BloonManager.SpawnBloon(bloonType);
		}, intervalSeconds, numBloons);
}

void GameWave::MultipleBloons(MultipleBloonsDesc bloons)
{
	MultipleBloons(bloons.bloonType, bloons.intervalSeconds, bloons.numBloons);
}