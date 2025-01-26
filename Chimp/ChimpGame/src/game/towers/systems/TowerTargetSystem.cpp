#include "TowerTargetSystem.h"

TowerTargetSystem::TowerTargetSystem(Chimp::Engine& engine, Chimp::ECS& ecs) :
	m_Engine(engine),
	m_ECS(ecs)
{
}

void TowerTargetSystem::Update()
{
	// Get all bloons
	std::vector<Chimp::Vector2f> bloonPositions;
	std::vector<Chimp::EntityId> bloonIds;
	{
		auto bloonsView = m_ECS.GetEntitiesWithComponents<BloonComponent, Chimp::EntityIdComponent, Chimp::TransformComponent>();
		bloonPositions.reserve(bloonsView.Size());
		bloonIds.reserve(bloonsView.Size());
		for (size_t i = 0; i < bloonsView.Size(); i++) {
			auto [bloon, bloonEntityId, bloonTransform] = bloonsView.GetByIndex(i);
			bloonPositions.push_back({ bloonTransform.GetTranslation().x, bloonTransform.GetTranslation().y });
			bloonIds.push_back(bloonEntityId.Id);
		}
	}

	// Get all towers
	auto towersView = m_ECS.GetEntitiesWithComponents<TowerComponent, Chimp::TransformComponent>();

	// Find closest bloon to each tower
	std::vector<std::function<void()>> tasks;
	for (size_t i = 0; i < towersView.Size(); i++) {
		auto task = [i, &towersView, &bloonPositions, &bloonIds]() {
			auto [tower, towerTransform] = towersView.GetByIndex(i);
			tower.Target.Reset();
			if (!bloonPositions.empty()) {
				const size_t closestBloonIndex = Chimp::FindClosest({ towerTransform.GetTranslation().x, towerTransform.GetTranslation().y }, bloonPositions);
				tower.Target = bloonIds[closestBloonIndex];
			}
			};
		tasks.push_back(task);
	}

	m_Engine.GetThreadPool().WaitUntilTasksExecuted(tasks);
}