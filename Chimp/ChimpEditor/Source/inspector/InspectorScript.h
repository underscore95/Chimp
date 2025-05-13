#pragma once

#include "Chimp.h"

namespace ChimpEditor {
	class SceneHierarchyScript;
	class InspectorScript : public Chimp::IEntityScript {
	private:
		enum class ByteVisualisationFormat {
			None = 0, Hex, Binary
		};
	public:
		InspectorScript(Chimp::EntityId entity, Chimp::Engine& engine, Chimp::ECS& ecs, Chimp::ECS& gameEcs);

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		void RenderByteVisualisationOption();
		void RenderByteVisualisation(Chimp::AnyReference component);
		std::string GetComponentTypeName(Chimp::AnyReference component);
	private:
		Chimp::ECS& m_gameEcs;
		Chimp::Reference< SceneHierarchyScript> m_sceneHierarchy;
		ByteVisualisationFormat m_byteVisualisationFormat = ByteVisualisationFormat::None;
	};
}