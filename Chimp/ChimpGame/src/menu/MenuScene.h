#pragma once

#include "stdafx.h"
#include "Settings.h"
#include "networking/Networking.h"

class MenuScene : public Chimp::Scene {
public:
	MenuScene(Chimp::Engine& engine, std::shared_ptr<Chimp::GameShader> shader = nullptr);
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
	std::shared_ptr<Chimp::GameShader> m_GameShader;
	Chimp::ConnectionInfo m_ConnectionInfo;
	Chimp::YAMLBlock m_MapList;
	Settings m_Settings;
};