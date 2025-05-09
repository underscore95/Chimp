#pragma once

#include "Chimp.h"

namespace ChimpEditor {
	class SceneHierarchyScript;
	class InspectorScript : public Chimp::IEntityScript {
	public:
		InspectorScript(Chimp::EntityId entity, Chimp::Engine& engine, Chimp::ECS& ecs, Chimp::ECS& gameEcs);

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		std::string GetComponentTypeName(Chimp::AnyReference component);
	private:
		Chimp::ECS& m_gameEcs;
		Chimp::Reference< SceneHierarchyScript> m_sceneHierarchy;
	};
}