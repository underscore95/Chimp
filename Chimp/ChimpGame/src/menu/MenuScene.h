#pragma once

#include "stdafx.h"

class MenuScene : public Chimp::Scene {
public:
	MenuScene(Chimp::Engine& engine);
	~MenuScene();

public:

protected:
	void OnInit() override;
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;

	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

	void LoadResources() override;
	void UnloadResources() override;

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS m_ECS;
	Chimp::Reference<Chimp::Mesh> m_TestMesh;
	Chimp::FreeCameraController m_Controller;
	Chimp::CameraPerspective m_Camera;

	Chimp::ModelResourcePath m_ModelPath;
};