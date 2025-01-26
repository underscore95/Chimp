#pragma once

#include "stdafx.h"


class GameOverScene : public Chimp::Scene {
public:
	GameOverScene(Chimp::Engine& engine,
		std::shared_ptr<Chimp::GameShader> gameShader,
		bool didWin);
	~GameOverScene();

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
	bool m_DidWin;
};