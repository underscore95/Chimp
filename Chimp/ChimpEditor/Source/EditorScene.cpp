#include "EditorScene.h"

namespace ChimpEditor {
	EditorScene::EditorScene(Chimp::Engine& engine) :
		m_engine(engine),
		m_sceneView(engine.GetRenderingManager().CreateRenderTexture(1920, 1080))
	{
		m_engine.GetWindow().SetTitle("Chimp Editor");
		m_engine.GetWindow().SetSize({ 1280, 720 });
		m_engine.GetWindow().SetResizable(true);

		GetLogger().Info("Editor launched.");
	}

	void EditorScene::OnInit() {
		m_engine.GetRenderingManager().SetDefaultRenderTarget(m_sceneView);
	}

	void EditorScene::OnActivate(std::unique_ptr<Scene> previousScene) {}

	void EditorScene::OnDeactivate() {}

	void EditorScene::OnUpdate() {}

	void EditorScene::OnRender() {
		auto& rm = m_engine.GetRenderingManager();
		rm.BindDefaultRenderTarget();
		rm.GetRenderer().SetClearColor(0.3f, 0.3f, 0.6f);
		rm.ClearColorBuffer();
		rm.ClearDepthBuffer();

		rm.SetFrameBuffer();
		rm.SetViewport({ 0,0 }, m_engine.GetWindow().GetSize());

		rm.GetRenderer().SetClearColor(0, 0, 0);
		rm.ClearDepthBuffer();
		rm.ClearColorBuffer();
	}

	void EditorScene::OnRenderUI() {
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize(m_engine.GetWindow().GetSize());
		ImGui::Begin("Chimp Editor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		ImVec2 sceneViewSize = m_engine.GetWindow().GetSize() * 0.5f;
		ImGui::Begin("Scene View");
		ImGui::Image(m_sceneView->GetImGuiImageHandle(), sceneViewSize);
		ImGui::End();

		ImGui::End();
	}

	void EditorScene::LoadResources() {}

	void EditorScene::UnloadResources() {}
}