#pragma once

#include "Chimp.h"

namespace ChimpEditor {
	class HierarchyComponent;

	class SceneHierarchyScript : public Chimp::IEntityScript {
	public:
		SceneHierarchyScript(Chimp::Engine& engine, Chimp::ECS& ecs, Chimp::ECS& gameECS);

	public:
		Chimp::EntityId GetSelectedEntity() const;
		bool HasSelectedEntity() const;

		void SelectEntity(Chimp::EntityId entity);
	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		void RenderUI(Chimp::EntityId entity, const Chimp::HierarchyComponent& hierarchyComp, float cursorPosX);


	private:
		Chimp::ECS& m_gameECS;
		Chimp::EntityId m_selectedEntity;
		bool m_hasSelectedEntity = false;
	};
}