#include "TowerUpgradeDisplaySystem.h"

TowerUpgradeDisplaySystem::TowerUpgradeDisplaySystem(Chimp::Engine& engine,
	Chimp::ECS& ecs) :
	m_Engine(engine),
	m_ECS(ecs)
{
}

void TowerUpgradeDisplaySystem::RenderUI()
{
	auto view = m_ECS.GetEntitiesWithComponents<TowerComponent, UpgradableComponent, Chimp::TransformComponent>();
	for (auto& [tower, upgrade, transform] : view) {
		Chimp::Vector2f pos = { transform.GetTranslation().x, -transform.GetTranslation().y - 50 };
		std::string text = "";
		if (upgrade.NumDamageUpgrades > 0) {
			text += "DMG: " + std::to_string(upgrade.NumDamageUpgrades) + " ";
		}
		if (upgrade.NumAttackSpeedUpgrades > 0) {
			if (upgrade.NumDamageUpgrades > 0) {
				text += "| ";
				pos.x -= 25;
			}
			text += "SPD: " + std::to_string(upgrade.NumAttackSpeedUpgrades);
		}
		ImGui::SetCursorPos({ pos.x, pos.y });
		ImGui::Text(text.c_str());
	}
}