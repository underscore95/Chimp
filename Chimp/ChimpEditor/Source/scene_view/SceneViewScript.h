#pragma once

#include "Chimp.h"

namespace ChimpEditor {
	class SceneViewScript : public Chimp::IEntityScript {
	public:
		SceneViewScript(Chimp::EntityId entity, Chimp::Engine& engine, Chimp::ECS& ecs);

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

	private:
		std::shared_ptr<Chimp::IRenderTexture> m_sceneViewTexture;
	};
}