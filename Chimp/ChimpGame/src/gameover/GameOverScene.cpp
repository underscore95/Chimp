#include "GameOverScene.h"
#include "menu/MenuScene.h"
#include "networking/GameClient.h"
#include "networking/GameServer.h"

GameOverScene::GameOverScene(Chimp::Engine& engine, 
	std::shared_ptr<Chimp::GameShader>  gameShader,
	bool didWin) :
	m_Engine(engine),
	m_GameShader(gameShader),
	m_DidWin(didWin)
{
	LoadResources();
}

GameOverScene::~GameOverScene()
{
	UnloadResources();
}

void GameOverScene::OnInit()
{

}

void GameOverScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
}

void GameOverScene::OnDeactivate()
{
}

void GameOverScene::OnUpdate()
{
}

void GameOverScene::OnRender()
{
}

void GameOverScene::OnRenderUI()
{
	ImGui::Text(m_DidWin ? "You won!" : "You lost!");
	if (ImGui::Button("Go To Main Menu")) {
		m_Engine.GetSceneManager().QueueSceneChange(std::make_unique<MenuScene>(m_Engine, m_GameShader));
	}
}

void GameOverScene::LoadResources()
{
}

void GameOverScene::UnloadResources()
{
}
