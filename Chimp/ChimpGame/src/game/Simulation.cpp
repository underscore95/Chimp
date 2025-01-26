#include "Simulation.h"
#include "networking/Networking.h"

Simulation::Simulation(Chimp::Engine& engine,
	std::shared_ptr<Chimp::GameShader> gameShader,
	Chimp::Vector2f position,
	Chimp::Vector2f size,
	bool isPlayerSimulation,
	MoneyManager& moneyManager,
	Chimp::YAMLBlock& currentMap) :
	m_Engine(engine),
	m_GameShader(gameShader),
	m_Position(position),
	m_Size(size),
	m_HealthSystem(m_ECS),
	m_BloonManager(m_Engine, m_ECS, m_Position, currentMap),
	m_WaveManager(WaveManagerBuilder::Build(m_Engine, m_BloonManager)),
	m_TowerManager(m_Engine, m_ECS, m_Position, m_Size),
	m_IsPlayerSimulation(isPlayerSimulation),
	m_MoneyManager(moneyManager),
	m_CurrentMap(currentMap)
{
	if (isPlayerSimulation) {
		m_TowerEditor = std::make_unique<TowerEditor>(m_TowerManager, m_Engine, m_ECS, m_Position, m_Size, m_MoneyManager, m_CurrentMap);
	}
}

void Simulation::Init()
{
	Entities::CreateBaseEntity(
		m_ECS,
		m_Engine.GetResourceManager().GetSprites().Get(GAME_DATA_FOLDER + m_CurrentMap.Strings["Background"]),
		{
			{ m_Position.x, m_Position.y, 10.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ m_Engine.GetWindow().GetSize().x / 2.0f, m_Engine.GetWindow().GetSize().y, 500.0f}
		}
	);
}

void Simulation::Update()
{
	m_HealthSystem.Update();
	m_BloonManager.Update();
	m_WaveManager->Update();
	m_TowerManager.Update();
	if (m_TowerEditor) {
		m_TowerEditor->Update();
	}
	if (!m_IsPlayerSimulation) {
		Networking::GetClient()->GetHandlers().TowerListener->Update(m_TowerManager);
	}
}

void Simulation::Render()
{
	m_GameShader->RenderWorld(m_ECS);
}

void Simulation::RenderUI()
{
	m_BloonManager.RenderUI();
	if (m_TowerEditor) {
		m_TowerEditor->RenderUI();
	}
	m_TowerManager.RenderUI();
}

bool Simulation::HasLost() const
{
	return m_BloonManager.HasLost() || !Networking::GetClient()->IsConnected();
}

Chimp::WaveManager& Simulation::GetWaveManager()
{
	return *m_WaveManager;
}

TowerManager& Simulation::GetTowerManager()
{
	return m_TowerManager;
}

Chimp::Vector2f Simulation::GetPosition() const
{
	return m_Position;
}

BloonManager& Simulation::GetBloonManager()
{
	return m_BloonManager;
}
