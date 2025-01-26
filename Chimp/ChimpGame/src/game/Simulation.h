#pragma once

#include "stdafx.h"

#include "Entities.h"
#include "entities/health/HealthSystem.h"
#include "bloons/BloonManager.h"
#include "waves/WaveManagerBuilder.h"
#include "towers/TowerManager.h"
#include "towers/TowerEditor.h"

class Simulation {
public:
	Simulation(Chimp::Engine& engine, 
		std::shared_ptr<Chimp::GameShader> gameShader,
		Chimp::Vector2f position,
		Chimp::Vector2f size,
		bool isPlayerSimulation,
		MoneyManager& moneyManager,
		Chimp::YAMLBlock& currentMap);
	void Init();

	void Update();
	void Render();
	void RenderUI();

	bool HasLost() const;
	Chimp::WaveManager& GetWaveManager();
	TowerManager& GetTowerManager();
	Chimp::Vector2f GetPosition() const;
	BloonManager& GetBloonManager();
private:
	Chimp::Engine& m_Engine;
	Chimp::Vector2f m_Position;
	Chimp::Vector2f m_Size;
	Chimp::ECS m_ECS;
	std::shared_ptr<Chimp::GameShader> m_GameShader;
	HealthSystem m_HealthSystem;
	BloonManager m_BloonManager;
	std::unique_ptr<Chimp::WaveManager> m_WaveManager;
	TowerManager m_TowerManager;
	std::unique_ptr<TowerEditor> m_TowerEditor;
	bool m_IsPlayerSimulation;
	MoneyManager& m_MoneyManager;
	Chimp::YAMLBlock& m_CurrentMap;
};