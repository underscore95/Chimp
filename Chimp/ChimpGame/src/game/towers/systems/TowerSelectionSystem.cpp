#include "TowerSelectionSystem.h"

TowerSelectionSystem::TowerSelectionSystem(Chimp::Engine& engine,
	Chimp::ECS& ecs,
	Chimp::Vector2f simulationPosition) :
	m_Engine(engine),
	m_ECS(ecs),
	m_SimulationPosition(simulationPosition)
{
}

void TowerSelectionSystem::Update()
{
	if (!m_Engine.GetWindow().GetInputManager().IsMouseButtonPressed(Chimp::Mouse::LEFT)) return;

	auto clickedPos = m_Engine.GetWindow().GetInputManager().GetMousePosition();
	clickedPos.y *= -1;
	assert(m_SimulationPosition.y == 0); // if not, might need to add before flipping, just make sure its correct

	auto view = m_ECS.GetEntitiesWithComponents<TowerComponent, Chimp::TransformComponent, Chimp::EntityIdComponent>();
	for (auto& [tower, transform, id] : view) {
		Chimp::Rect towerBounds = {
			{transform.GetTranslation().x, transform.GetTranslation().y},
			{transform.GetScale().x, transform.GetScale().y}
		};
		towerBounds.Scale(2.0f);

		if (towerBounds.Contains(clickedPos)) {
			m_SelectedTower = id.Id;
			break;
		}
	};
}

void TowerSelectionSystem::RenderUI()
{
	if (!IsTowerSelected()) return;

	auto transform = m_ECS.GetComponent<Chimp::TransformComponent>(m_SelectedTower.value());
	assert(transform.HasValue());
	Chimp::Vector2f position = { transform->GetTranslation().x, -transform->GetTranslation().y };

	ImGui::SetCursorPos({ position.x + 10, position.y + 10 });
	ImGui::Text("Selected");
}

bool TowerSelectionSystem::IsTowerSelected()
{
	if (m_SelectedTower.has_value()) {
		if (!m_ECS.IsEntityAlive(m_SelectedTower.value())) {
			m_SelectedTower.reset();
			return false;
		}
		return true;
	}
	else {
		return false;
	}
}

Chimp::EntityId TowerSelectionSystem::GetSelectedTower() const
{
	assert(m_SelectedTower.has_value());
	return m_SelectedTower.value();
}
