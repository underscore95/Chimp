#include "GameScene.h"
#include "Entities.h"
#include "networking/Networking.h"

GameScene::GameScene(Chimp::Engine& engine,
	std::shared_ptr<Chimp::GameShader> gameShader,
	const std::string& map) :
	m_Engine(engine),
	m_GameShader(gameShader),
	m_TaskScheduler(engine.CreateTaskScheduler()),
	m_CurrentMap(Chimp::YAMLBlockParser::Parse(GAME_DATA_FOLDER + std::string("/Maps/") + map + ".yml").Data)
{
	const auto simulationSize = Chimp::ComponentMultiply(m_Engine.GetWindow().GetSize(), { 0.5, 1.0 });
	m_OpponentSimulation = std::make_unique<Simulation>(engine, m_GameShader, Chimp::Vector2f{ 0.0f, 0.0f }, simulationSize, false, m_MoneyManager, m_CurrentMap);
	m_PlayerSimulation = std::make_unique<Simulation>(engine, m_GameShader, Chimp::Vector2f{ m_Engine.GetWindow().GetSize().x / 2.0f, 0.0f }, simulationSize, true, m_MoneyManager, m_CurrentMap);

	m_WaveStartHandler = std::make_unique<WaveStartHandler>(m_PlayerSimulation->GetWaveManager(), m_OpponentSimulation->GetWaveManager(), *m_TaskScheduler);

	m_MatchWinLoseHandler = std::make_unique<MatchWinLoseHandler>(m_Engine, *m_PlayerSimulation, m_GameShader);

	m_BloonSpawner = std::make_unique<BloonSpawner>(m_Engine, m_OpponentSimulation->GetBloonManager(), m_MoneyManager, *m_TaskScheduler, m_PlayerSimulation->GetWaveManager());

	m_GameRunningTimer.Start();

	LoadResources();
}

GameScene::~GameScene()
{
	UnloadResources();
}

void GameScene::OnInit()
{
	m_PlayerSimulation->Init();
	m_OpponentSimulation->Init();
}

void GameScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
	m_Engine.GetMusicPlayer().SwitchMusic(Chimp::MusicTracksContainer{ {
	GAME_DATA_FOLDER + std::string("/Assets/Music/game.wav"),
	GAME_DATA_FOLDER + std::string("/Assets/Music/game2.wav")
	} });
}

void GameScene::OnDeactivate()
{
}

void GameScene::OnUpdate()
{
	m_PlayerSimulation->Update();
	m_OpponentSimulation->Update();

	m_WaveStartHandler->Update();

	m_MatchWinLoseHandler->Update();

	m_BloonSpawner->Update();

	m_MoneyManager.Update();

	Networking::GetClient()->GetHandlers().BloonListener->Update(m_PlayerSimulation->GetBloonManager());

	m_TaskScheduler->Update();
}

void GameScene::OnRender()
{
	m_GameShader->BeginFrame();

	m_PlayerSimulation->Render();
	m_OpponentSimulation->Render();
}

void GameScene::OnRenderUI()
{
	ImGui::Begin("##GameSceneUI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::SetWindowPos({ 0.0f, 0.0f });
	ImGui::SetWindowSize({ m_Engine.GetWindow().GetSize().x, m_Engine.GetWindow().GetSize().y });

	m_PlayerSimulation->RenderUI();
	m_OpponentSimulation->RenderUI();

	ImGui::SetWindowFontScale(2.5f);
	ImGui::SetCursorPosY(100);
	m_Engine.GetImGuiHelper().CenteredTextHorizontally(std::format("Wave: {} / {}", Chimp::Min(m_PlayerSimulation->GetWaveManager().GetWave(), m_PlayerSimulation->GetWaveManager().NumWaves()), m_PlayerSimulation->GetWaveManager().NumWaves()));
	ImGui::SetWindowFontScale(2.0f);
	m_Engine.GetImGuiHelper().CenteredTextHorizontally(m_Engine.GetTimeManager().FormatTime(m_GameRunningTimer.GetSecondsElapsed()));
	m_Engine.GetImGuiHelper().CenteredTextHorizontally(std::format("${} (+${}/{}s)", m_MoneyManager.GetMoney(), m_MoneyManager.GetIncome(), MoneyManager::IncomeInterval));
	ImGui::SetWindowFontScale(1.0f);

	m_BloonSpawner->RenderUI();
	ImGui::End();
}

void GameScene::LoadResources()
{
	auto& sprites = m_Engine.GetResourceManager().GetSprites();
	sprites.Depend(GAME_DATA_FOLDER + std::string("/Assets/Textures/ErrorTowerOverlay.png"));
	sprites.Depend(GAME_DATA_FOLDER + std::string("/Assets/Textures/Dart.png"));
	for (size_t i = 0; i < Bloons::NUM_BLOON_TYPES; ++i) {
		sprites.Depend(GAME_DATA_FOLDER + std::string("/Assets/Textures/") + Bloons::TexturePaths[i]);
	}

	sprites.Depend(GAME_DATA_FOLDER + m_CurrentMap.Strings["Background"]);
	m_Engine.GetResourceManager().GetImages().Depend(GAME_DATA_FOLDER + m_CurrentMap.Strings["Collisions"]);

	m_Engine.GetResourceManager().GetSoundEffects().Depend(GAME_DATA_FOLDER + std::string("/Assets/Sounds/Bloon.yml"));
	m_Engine.GetResourceManager().GetSoundEffects().Depend(GAME_DATA_FOLDER + std::string("/Assets/Sounds/Place.yml"));

	// Models
	Chimp::IModelImporter::Settings settings;
	settings.FlipUVs = false;
	settings.IncludeNormals = false;
	settings.IncludeTextureCoordinates = true;

	m_Engine.GetResourceManager().GetModels().ImportSettings = settings;
	m_Engine.GetResourceManager().GetModels().Depend(std::string(GAME_DATA_FOLDER) + "/Assets/Models/monkey/MonkeyOBJ.obj");
}

void GameScene::UnloadResources()
{
	auto& sprites = m_Engine.GetResourceManager().GetSprites();

	sprites.Release(GAME_DATA_FOLDER + m_CurrentMap.Strings["Background"]);
	m_Engine.GetResourceManager().GetImages().Release(GAME_DATA_FOLDER + m_CurrentMap.Strings["Collisions"]);

	sprites.Release(GAME_DATA_FOLDER + std::string("/Assets/Textures/Dart.png"));
	sprites.Release(GAME_DATA_FOLDER + std::string("/Assets/Textures/ErrorTowerOverlay.png"));
	for (size_t i = 0; i < Bloons::NUM_BLOON_TYPES; ++i) {
		sprites.Release(GAME_DATA_FOLDER + std::string("/Assets/Textures/") + Bloons::TexturePaths[i]);
	}

	m_Engine.GetResourceManager().GetModels().Release(std::string(GAME_DATA_FOLDER) + "/Assets/Models/monkey/MonkeyOBJ.obj");
}

bool GameScene::ShouldExit(Chimp::Engine& engine) const
{
	return m_Engine.GetWindow().GetInputManager().IsKeyDown(Chimp::Keyboard::ESCAPE)
		|| Scene::ShouldExit(engine);
}