#include "MenuScene.h"
#include "Logger.h"

MenuScene::MenuScene(Chimp::Engine& engine)
	: m_Engine(engine)
{
	LoadResources();

	m_Engine.GetWindow().SetTitle("Chimp Demo");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);


	auto& renderingManager = m_Engine.GetRenderingManager();

		m_GameShader = std::make_shared<Chimp::GameShader>(m_Engine, Chimp::ShaderFilePaths{
			GAME_DATA_FOLDER + std::string("/Assets/Shaders/Default.vert"),
			GAME_DATA_FOLDER + std::string("/Assets/Shaders/Default.frag")
			});
}

MenuScene::~MenuScene()
{
	UnloadResources();
}

void MenuScene::OnInit()
{
}

void MenuScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{

}

void MenuScene::OnDeactivate()
{
}

void MenuScene::OnUpdate()
{
	
}

void MenuScene::OnRender()
{

}

void MenuScene::OnRenderUI()
{
	
}

void MenuScene::LoadResources()
{
}

void MenuScene::UnloadResources()
{
}
