#include "Game.h"
#include "TestScript.h"
#include "MainScene.h"

void Game::Setup(Chimp::Engine& engine)
{
	CHIMP_GAME_REGISTER_SCRIPT(Test, TestScript);

	GetLogger().Info("Setup game");
}

std::unique_ptr<Chimp::Scene> Game::CreateRuntimeEntryScene(Chimp::Engine& engine)
{
	return UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::Scene, new MainScene(engine));
}
