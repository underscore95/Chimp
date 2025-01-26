#include "TowerEditor.h"
#include "networking/Networking.h"

TowerEditor::TowerEditor(TowerManager& towerManager,
	Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition,
	Chimp::Vector2f simulationSize,
	MoneyManager& moneyManager,
	Chimp::YAMLBlock& currentMap) :
	m_TowerManager(towerManager),
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition),
	m_SimulationSize(simulationSize),
	m_SelectionSystem(engine, ecs, simulationPosition),
	m_MoneyManager(moneyManager),
	m_DeselectTexture(engine.GetResourceManager().GetTextures(), GAME_DATA_FOLDER + std::string("/Assets/Textures/X.png")),
	m_CurrentMap(currentMap)
{
	m_TowerIconTextures.reserve(NUM_TOWERS);
	for (TowerType type = 0; type < NUM_TOWERS; type++) {
		m_TowerIconTextures.push_back(Chimp::TextureDependency(engine.GetResourceManager().GetTextures(), TOWER_ICONS[type]));
	}
}

void TowerEditor::Update()
{
	if (m_IsPlacing) {
		m_SelectionSystem.DeselectTower();
	}
	else {
		m_SelectionSystem.Update();
	}

	// Prevent placing if no money
	m_IsPlacing = m_IsPlacing && m_MoneyManager.HasMoney(TOWER_COSTS[m_PlacingType]);

	// Handle placing towers
	auto mousePos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
	if (m_Engine.GetWindow().GetInputManager().IsMouseButtonPressed(Chimp::Mouse::LEFT)
		&& m_IsPlacing
		&& mousePos >= m_SimulationPosition
		&& mousePos <= m_SimulationPosition + m_SimulationSize
		&& !CollidesWithTower(Chimp::ComponentMultiply(mousePos, Chimp::Vector2f{ 1,-1 }))
		&& !CollidesWithTrack(mousePos - m_SimulationPosition)) {
		auto towerPos = Chimp::ComponentMax({ 0,0 }, mousePos - m_SimulationPosition);
		towerPos.y *= -1;
		Place(m_PlacingType, towerPos);
		m_MoneyManager.RemoveMoney(TOWER_COSTS[m_PlacingType]);
		m_IsPlacing = false;
	}
}

void TowerEditor::RenderUI()
{
	auto errorTowerOverlay = (intptr_t)m_Engine.GetResourceManager().GetTextures().Get(GAME_DATA_FOLDER + std::string("/Assets/Textures/ErrorTowerOverlay.png")).GetId();
	if (m_IsPlacing) {
		ImGui::SetCursorPos({ 0,0 });
		ImGui::Image(errorTowerOverlay, { m_SimulationPosition.x, m_Engine.GetWindow().GetSize().y });
	}

	const Chimp::Vector2f towerIconSize = { TOWER_ICON_SIZE, TOWER_ICON_SIZE };

	Chimp::Vector2f iconPosition = ImGui::GetWindowSize();
	iconPosition.x /= 2;
	iconPosition.x -= 60.0f * NUM_TOWERS / 2.0f;
	iconPosition.y -= 100;

	// DESELECT BUTTON
	if (m_IsPlacing) {
		ImGui::SetCursorPos({ iconPosition.x - 100, iconPosition.y });
		intptr_t deselect = (intptr_t)m_DeselectTexture.GetResource().GetId();
		if (ImGui::ImageButton("##close", deselect, ImVec2(50, 50))) {
			m_IsPlacing = false;
		}
	}

	for (TowerType type = 0; type < NUM_TOWERS; type++) {
		intptr_t icon = (intptr_t)m_TowerIconTextures[type].GetResource().GetId();

		// DRAW SELECT TOWER TO PLACE BUTTON
		ImGui::SetCursorPos(iconPosition);
		std::string label = "##tower" + std::to_string(type);
		if (!m_MoneyManager.HasMoney(TOWER_COSTS[type])) {
			ImGui::BeginDisabled();
		}
		if (ImGui::ImageButton(label.c_str(), icon, ImVec2(50, 50))) {
			m_IsPlacing = true;
			m_PlacingType = type;
		}
		if (!m_MoneyManager.HasMoney(TOWER_COSTS[type])) {
			ImGui::EndDisabled();
		}

		iconPosition.x += 60.0f;

		// DRAW PREVIEW PLACE
		if (m_IsPlacing && m_PlacingType == type) {
			auto mousePos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
			ImGui::SetCursorPos({ mousePos.x, mousePos.y });
			ImGui::Image(icon, towerIconSize);
			if (
				CollidesWithTrack(mousePos - m_SimulationPosition) || 
				CollidesWithTower(Chimp::ComponentMultiply(mousePos, Chimp::Vector2f{ 1,-1 })) ||
				mousePos.x < m_SimulationPosition.x || mousePos.x > m_SimulationPosition.x + m_SimulationSize.x ||
				mousePos.y < m_SimulationPosition.y || mousePos.y > m_SimulationPosition.y + m_SimulationSize.y
				) {
				ImGui::SetCursorPos({ mousePos.x, mousePos.y });
				ImGui::Image(errorTowerOverlay, towerIconSize);
			}
		}
	}

	// DRAW SELECTED TOWER UI
	m_SelectionSystem.RenderUI();

	if (m_SelectionSystem.IsTowerSelected()) {
		auto selectedTower = m_SelectionSystem.GetSelectedTower();
		auto upgrades = m_ECS.GetMutableComponent<UpgradableComponent>(selectedTower);
		assert(upgrades.HasValue());

		// REMOVE BUTTON
		ImGui::SetCursorPos({ iconPosition.x + 60, iconPosition.y });
		const int worth = (int)((float)m_ECS.GetMutableComponent<WorthComponent>(selectedTower)->Worth * WorthComponent::SELL_MULTIPLIER);
		const std::string removeButtonLabel = "Sell +$" + std::to_string(worth) + "##removeTower";
		if (ImGui::Button(removeButtonLabel.c_str(), ImVec2(100, 50))) {
			RemoveSelectedTower();
			return;
		}

		// DESELECT BUTTON
		ImGui::SetCursorPos({ iconPosition.x + 60, iconPosition.y - 60 });
		if (ImGui::Button("Deselect Tower##deselectTower", ImVec2(100, 50))) {
			m_SelectionSystem.DeselectTower();
			return;
		}

		// UPGRADE DAMAGE BUTTON
		ImGui::SetCursorPos({ iconPosition.x + 60 + 110, iconPosition.y });
		std::stringstream ss;
		ss << "Upgrade Damage: " << upgrades->GetDamageUpgradeCost() << "##upgradeDamage";
		 bool canNotAfford = !m_MoneyManager.HasMoney(upgrades->GetDamageUpgradeCost());
		 if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		if (ImGui::Button(ss.str().c_str(), ImVec2(200, 50))) {
			UpgradeSelectedTower(UpgradeType::ATTACK_DAMAGE);
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}

		// UPGRADE ATTACK SPEED BUTTON
		ImGui::SetCursorPos({ iconPosition.x + 60 + 110, iconPosition.y - 60 });
		ss.str("");
		ss << "Upgrade Attack Speed: " << upgrades->GetAttackSpeedUpgradeCost() << "##upgradeAttackSpeed";
		  canNotAfford = !m_MoneyManager.HasMoney(upgrades->GetAttackSpeedUpgradeCost());
		if (canNotAfford) {
			ImGui::BeginDisabled();
		}
		if (ImGui::Button(ss.str().c_str(), ImVec2(200, 50))) {
			UpgradeSelectedTower(UpgradeType::ATTACK_SPEED);
		}
		if (canNotAfford) {
			ImGui::EndDisabled();
		}
	}
}

void TowerEditor::Place(TowerType type, Chimp::Vector2f position)
{
	auto entityId = m_TowerManager.PlaceTower(type, position);
	auto towerId = m_ECS.GetComponent<NetworkedIdentifierComponent>(entityId)->Id;

	ClientTowerPlacePacket packet;
	packet.PacketType = Networking::CLIENT_TOWER_PLACE;
	packet.Position = position;
	packet.Type = type;
	packet.TowerId = towerId;

#ifndef DEBUG_AUTOSTART_WITH_1_PLAYER
	const auto opponentId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetOpponentId();
	Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
#endif
}

void TowerEditor::RemoveSelectedTower()
{
	if (!m_SelectionSystem.IsTowerSelected()) {
		return;
	}

	auto selectedTower = m_SelectionSystem.GetSelectedTower();
	auto towerId = m_ECS.GetComponent<NetworkedIdentifierComponent>(selectedTower)->Id;

	auto health = m_ECS.GetMutableComponent<Chimp::HealthComponent>(selectedTower);
	health->Health = 0;
	m_SelectionSystem.DeselectTower();

	auto worth = m_ECS.GetMutableComponent<WorthComponent>(selectedTower)->Worth;
	m_MoneyManager.AddMoney((int)((float)worth * WorthComponent::SELL_MULTIPLIER));

	ClientTowerRemovePacket packet;
	packet.PacketType = Networking::CLIENT_TOWER_REMOVE;
	packet.TowerId = towerId;

#ifndef DEBUG_AUTOSTART_WITH_1_PLAYER
	const auto opponentId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetOpponentId();
	Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
#endif
}

void TowerEditor::UpgradeSelectedTower(UpgradeType type)
{
	if (!m_SelectionSystem.IsTowerSelected()) {
		return;
	}

	auto selectedTower = m_SelectionSystem.GetSelectedTower();
	auto towerId = m_ECS.GetComponent<NetworkedIdentifierComponent>(selectedTower)->Id;

	auto upgrades = m_ECS.GetMutableComponent<UpgradableComponent>(selectedTower);
	auto worthComponent = m_ECS.GetMutableComponent<WorthComponent>(selectedTower);
	worthComponent->Worth += upgrades->GetUpgradeCost(type);
	m_MoneyManager.RemoveMoney(upgrades->GetUpgradeCost(type));

	upgrades->Upgrade(type); 

	// Packet
	ClientTowerUpgradePacket packet;
	packet.PacketType = Networking::CLIENT_TOWER_UPGRADE;
	packet.TowerId = towerId;
	packet.UpgradeType = type;

#ifndef DEBUG_AUTOSTART_WITH_1_PLAYER
	const auto opponentId = Networking::GetClient()->GetHandlers().CurrentMatchHandler->GetOpponentId();
	Networking::GetClient()->GetClient().SendPacketToClient(opponentId, packet);
#endif
}

bool TowerEditor::CollidesWithTrack(Chimp::Vector2i pos) const
{
	auto& collisions = m_Engine.GetResourceManager().GetImages().Get(GAME_DATA_FOLDER + m_CurrentMap.Strings["Collisions"]);

	return collisions.GetPixel({ collisions.Width - pos.x, collisions.Height - pos.y }).w > 0;
}

bool TowerEditor::CollidesWithTower(Chimp::Vector2f pos) const
{
	auto view = m_ECS.GetEntitiesWithComponents<Chimp::TransformComponent, TowerComponent>();
	for (auto& [transform, tower] : view) {
		if (Chimp::GetDistanceBetween(Chimp::Vector2f(transform.GetTranslation()), pos) < TOWER_ICON_SIZE * 0.5f) {
			return true;
		}
	}
	return false;
}
