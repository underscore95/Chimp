#pragma once

#include "stdafx.h"

class MainScene : public Chimp::Scene {
public:
	MainScene(Chimp::Engine& engine);
	~MainScene();

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
	Chimp::FreeCameraController m_controller;
	Chimp::CameraPerspective m_camera;
};