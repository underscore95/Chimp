#include "EditorScene.h"
#include "scene_view/SceneViewScript.h"

namespace ChimpEditor {
	EditorScene::EditorScene(Chimp::Engine& engine) :
		m_engine(engine),
		m_ecs(engine.CreateECS())
	{
		m_engine.GetWindow().SetTitle("Chimp Editor");
		m_engine.GetWindow().SetSize({ 1280, 720 });
		m_engine.GetWindow().SetResizable(true);

		GetLogger().Info("Editor launched.");

		LoadResources();
	}

	EditorScene::~EditorScene()
	{
		UnloadResources();
	}

	void EditorScene::OnInit() {
		m_sceneView = m_ecs->CreateEntity();

		m_ecs->GetScripts().AttachScript(m_sceneView, UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new SceneViewScript(m_sceneView, m_engine, *m_ecs)));

		m_ecs->GetSystems().OnInit();
	}

	void EditorScene::OnActivate(std::unique_ptr<Scene> previousScene) {}

	void EditorScene::OnDeactivate() {}

	void EditorScene::OnUpdate() {
		m_ecs->GetSystems().OnUpdate();
	}

	void EditorScene::OnRender() {
		m_ecs->GetSystems().OnRender();

		auto& rm = m_engine.GetRenderingManager();

		rm.SetViewport({ 0,0 }, m_engine.GetWindow().GetSize());
		rm.SetFrameBuffer();
		rm.GetRenderer().SetClearColor(0, 0, 0);
		rm.ClearColorBuffer();
	}

	void EditorScene::OnRenderUI() {
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize(m_engine.GetWindow().GetSize());
		ImGui::Begin("Chimp Editor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		m_ecs->GetSystems().OnRenderUI();

		ImGui::End();
	}

	void EditorScene::LoadResources() {}

	void EditorScene::UnloadResources() {}
}