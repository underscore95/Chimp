#pragma once

#include "Chimp.h"

namespace ChimpEditor {
	class SceneHierarchyScript : public Chimp::IEntityScript {
	public:
		SceneHierarchyScript(Chimp::EntityId entity, Chimp::Engine& engine, Chimp::ECS& ecs, Chimp::ECS& gameECS);

	public:
		Chimp::EntityId GetSelectedEntity();
		bool HasSelectedEntity();

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		void RenderUI(Chimp::EntityId entity, Chimp::HierarchyComponent& hierarchyComp, float cursorPosX);

	private:
		std::shared_ptr<Chimp::IRenderTexture> m_sceneViewTexture;
		Chimp::ECS& m_gameECS;
		Chimp::EntityId m_selectedEntity;
	};
}