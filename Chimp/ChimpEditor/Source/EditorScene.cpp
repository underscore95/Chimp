#include "EditorScene.h"
#include "scene_view/SceneViewScript.h"
#include "scene_hierarchy/SceneHierarchyScript.h"
#include "scene_hierarchy/EntityNameComponent.h"
#include "inspector/InspectorScript.h"

namespace ChimpEditor {
	EditorScene::EditorScene(Chimp::Engine& engine) :
		m_engine(engine),
		m_ecs(engine.CreateECS()),
		m_gameEcs(engine.CreateECS()),
		m_modelPath(GAME_DATA_FOLDER + std::string("/Assets/Models/cube.obj")),
		m_controller(m_camera, engine.GetWindow().GetInputManager())
	{
		LoadResources();
	}

	EditorScene::~EditorScene()
	{
		UnloadResources();
	}

	void EditorScene::OnInit() {

		// Camera
		m_camera.SetPosition(Chimp::Vector3f{ 10, 6, 10 });
		m_camera.SetForwardVector(m_camera.GetPosition() * -1.0f); // Look at 0 0 0

		// Scene view
		m_sceneView = m_ecs->CreateEntity();
		m_ecs->GetScripts().AttachScript(m_sceneView, UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new SceneViewScript(m_sceneView, m_engine, *m_ecs, *m_gameEcs)));

		// Scene hierarchy
		m_ecs->GetScripts().AttachScript(m_ecs->CreateEntity(), UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new SceneHierarchyScript(m_sceneView, m_engine, *m_ecs, *m_gameEcs)));

		// Inspector
		m_ecs->GetScripts().AttachScript(m_ecs->CreateEntity(), UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new InspectorScript(m_sceneView, m_engine, *m_ecs, *m_gameEcs)));

		// Testing
		m_gameEcs->GetHierarchy().SetParent(m_gameEcs->CreateEntity(), m_gameEcs->CreateEntity());
		auto testEnt = m_gameEcs->CreateEntity();
		m_gameEcs->SetComponent<EntityNameComponent>(testEnt, { "MyNamedEntity" });
		m_gameEcs->SetComponent<Chimp::TransformComponent>(testEnt, {});
		m_gameEcs->SetComponent<Chimp::MeshComponent>(testEnt, { &m_engine.GetResourceManager().GetModels().Get(m_modelPath) });
		m_gameEcs->SetComponent<Chimp::HealthComponent>(testEnt, { 10.0f });

		m_ecs->GetSystems().OnInit();
	}

	void EditorScene::OnActivate(std::unique_ptr<Scene> previousScene) {
		m_engine.GetWindow().SetTitle("Chimp Editor");
		m_engine.GetWindow().SetSize({ 1280, 720 });
		m_engine.GetWindow().SetResizable(true);

		auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
		shader.SetCamera(m_camera);
		shader.GetLighting().Ambient = { 1,1,1 };
		shader.GetLighting().NumPointLights = 0;
		shader.GetLighting().NumSpotlights = 0;
		shader.GetLighting().NumDirectionLights = 0;
	}

	void EditorScene::OnDeactivate() {}

	void EditorScene::OnUpdate() {
		m_ecs->GetSystems().OnUpdate();

		m_ecs->GetTransformManager().UpdateAllMatrices();
		m_gameEcs->GetTransformManager().UpdateAllMatrices();
	}

	void EditorScene::OnRender() {
		auto& rm = m_engine.GetRenderingManager();

		m_ecs->GetSystems().OnRender();

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

	void EditorScene::LoadResources() {
		m_engine.GetResourceManager().GetModels().Depend(m_modelPath);
	}

	void EditorScene::UnloadResources() {
		m_engine.GetResourceManager().GetModels().Release(m_modelPath);
	}
}