#include "Game.h"
#include "TestScript.h"

void Game::Setup(Chimp::Engine& engine)
{
	CHIMP_GAME_REGISTER_SCRIPT(Test, TestScript);

	GetLogger().Info("Setup game");
}