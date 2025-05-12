#pragma once

#include "stdafx.h"

namespace ChimpEditor {
	class EditorScene : public Chimp::Scene {
	public:
		EditorScene(Chimp::Engine& engine);
		~EditorScene();

		void OnInit() override;
		void OnActivate(std::unique_ptr<Scene> previousScene = nullptr) override;
		void OnDeactivate() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;
		void LoadResources() override;
		void UnloadResources() override;

	private:
		Chimp::Engine& m_engine;
		std::unique_ptr<Chimp::ECS> m_ecs;
		std::unique_ptr<Chimp::ECS> m_gameEcs;
		Chimp::EntityId m_sceneView;
		std::string m_modelPath;
		Chimp::FreeCameraController m_controller;
		Chimp::CameraPerspective m_camera;
	};
}