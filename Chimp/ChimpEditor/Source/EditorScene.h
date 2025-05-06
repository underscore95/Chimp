#pragma once

#include "stdafx.h"

namespace ChimpEditor {
	class EditorScene : public Chimp::Scene {
	public:
		EditorScene(Chimp::Engine& engine);

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
		std::shared_ptr<Chimp::IRenderTexture> m_sceneView;
	};
}