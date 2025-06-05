#include "EditorScene.h"
#include "scene_view/SceneViewScript.h"
#include "scene_hierarchy/SceneHierarchyScript.h"
#include "inspector/InspectorScript.h"
#include "assets/AssetManagerScript.h"
#include "assets/AssetImporterScript.h"
#include "api/IGame.h"
#include "Game.h" // This is necessary or IGame::Instance won't be set
#include "TestScript.h"

namespace ChimpEditor {
	EditorScene::EditorScene(Chimp::Engine& engine) :
		m_engine(engine),
		m_ecs(engine.CreateECS()),
		m_modelPath(GAME_DATA_FOLDER + std::string("/Assets/Models/cube.obj")),
		m_controller(m_camera, engine.GetWindow().GetInputManager())
	{
		LoadResources();
		Chimp::IGame::Instance().Setup(engine);
	}

	EditorScene::~EditorScene()
	{
		UnloadResources();
	}

	void EditorScene::OnInit() {
		// Camera
		m_camera.SetPosition(Chimp::Vector3f{ 10, 6, 10 });
		m_camera.SetForwardVector(m_camera.GetPosition() * -1.0f); // Look at 0 0 0

		// Load ECS
		GetLogger().Info("Loading entities...");
		if (!LoadGameECS()) {
			m_gameEcs = m_engine.CreateECS();
			m_gameEcs->GetHierarchy().SetParent(m_gameEcs->CreateEntity(), m_gameEcs->CreateEntity());
			auto testEnt = m_gameEcs->CreateEntity();
			m_gameEcs->SetComponent<Chimp::EntityNameComponent>(testEnt, { "MyNamedEntity" });
			m_gameEcs->SetComponent<Chimp::TransformComponent>(testEnt, {});
			m_gameEcs->SetComponent<Chimp::MeshComponent>(testEnt, { { &m_engine.GetResourceManager().GetModels().Get(m_modelPath), m_modelPath } });
			m_gameEcs->SetComponent<Chimp::HealthComponent>(testEnt, { 10.0f });
			m_gameEcs->GetScripts().AttachScript(testEnt, "Test");

			// Lighting entities
			{
				// Point Light Entity
				auto pointLightEnt = m_gameEcs->CreateEntity();
				m_gameEcs->SetComponent<Chimp::EntityNameComponent>(pointLightEnt, { "PointLightEntity" });
				m_gameEcs->SetComponent<Chimp::TransformComponent>(pointLightEnt, {
					{0.0f, 0.0f, -2.5f},
					{0, 0, 0},
					{1, 1, 1}
					});
				m_gameEcs->SetComponent<Chimp::PointLight>(pointLightEnt, {
					.Position = {0, 0, 0},
					.Color = {1, 1, 1},
					.Attenuation = {0.0f, 0.1f, 0.0f}
					});

				// Directional Light Entity
				auto dirLightEnt = m_gameEcs->CreateEntity();
				m_gameEcs->SetComponent<Chimp::EntityNameComponent>(dirLightEnt, { "DirectionalLightEntity" });
				m_gameEcs->SetComponent<Chimp::TransformComponent>(dirLightEnt, {
					{0.0f, 0.0f, 0.0f},
					{90, 0, 0},
					{1, 1, 1}
					});
				m_gameEcs->SetComponent<Chimp::DirectionalLight>(dirLightEnt, {
					.Direction = Chimp::Vector3f{0.0f, -1.0f, 0.0f}, // shouldn't be modified, transform rotation should be used instead
					.Color = Chimp::Vector3f{1.0f, 1.0f, 1.0f},
					.ShadowBias = 0.05f,
					.SqrtNumShadowSamples = 3
					});

				// Spotlight Entity
				auto spotLightEnt = m_gameEcs->CreateEntity();
				m_gameEcs->SetComponent<Chimp::EntityNameComponent>(spotLightEnt, { "SpotlightEntity" });
				m_gameEcs->SetComponent<Chimp::TransformComponent>(spotLightEnt, {
					{0.0f, 5.0f, 0.0f},
					{90, 0, 0},
					{1, 1, 1}
					});
				m_gameEcs->SetComponent<Chimp::Spotlight>(spotLightEnt, {
					.Direction = Chimp::Vector3f{0.0f, -1.0f, 0.0f}, // shouldn't be modified, transform rotation should be used instead
					.Position = Chimp::Vector3f{0.0f, 5.0f, 0.0f},
					.Color = Chimp::Vector3f{1.0f, 1.0f, 1.0f},
					.Attenuation = Chimp::Vector3f{0.0f, 0.1f, 0.0f},
					.ShadowBias = 0.05f,
					.SqrtNumShadowSamples = 3
					});

				// Ambient Light Entity
				auto ambientLightEnt = m_gameEcs->CreateEntity();
				m_gameEcs->SetComponent<Chimp::EntityNameComponent>(ambientLightEnt, { "AmbientLightEntity" });
				m_gameEcs->SetComponent<Chimp::AmbientLight>(ambientLightEnt, {
					.Color = {0.2f, 0.2f, 0.2f}
					});
			}
			GetLogger().Info("Failed to load ECS, created a new one!");
		}
		else {
			GetLogger().Info("Loaded ECS successfully!");
		}

		// Editor ECS
		GetLogger().Info("Below script warnings can be ignored vvv");

		// Scene view
		m_sceneView = m_ecs->CreateEntity();
		m_ecs->GetScripts().AttachScript(m_sceneView, UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new SceneViewScript(m_engine, *m_ecs, *m_gameEcs)));

		// Scene hierarchy
		m_ecs->GetScripts().AttachScript(m_ecs->CreateEntity(), UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new SceneHierarchyScript(m_engine, *m_ecs, *m_gameEcs)));

		// Inspector
		m_ecs->GetScripts().AttachScript(m_ecs->CreateEntity(), UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new InspectorScript(m_engine, *m_ecs, *m_gameEcs)));

		// Asset manager
		Chimp::Reference<AssetManagerScript> assetManager = { new AssetManagerScript(m_engine, *m_ecs) };
		m_ecs->GetScripts().AttachScript(m_ecs->CreateEntity(), UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, assetManager.GetNotNullPtr()));

		// Asset importer
		m_ecs->GetScripts().AttachScript(m_ecs->CreateEntity(), UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new AssetImporterScript(m_engine, *m_ecs, assetManager.Get())));

		GetLogger().Info("Above script warnings can be ignored ^^^");

		// Init
		m_ecs->GetSystems().OnInit();
	}

	void EditorScene::OnActivate(std::unique_ptr<Scene> previousScene) {
		m_engine.GetWindow().SetTitle("Chimp Editor");
		m_engine.GetWindow().SetSize({ 1280, 720 });
		m_engine.GetWindow().SetResizable(true);

		auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
		shader.SetCamera(m_camera);
	}

	void EditorScene::OnDeactivate() {
		SaveGameECS();
	}

	void EditorScene::OnUpdate() {
		m_ecs->GetSystems().OnUpdate();

		m_ecs->GetTransformManager().UpdateAllMatrices();
		m_gameEcs->GetTransformManager().UpdateAllMatrices();
	}

	void EditorScene::OnRender() {
		auto& rm = m_engine.GetRenderingManager();
		auto& shader = m_engine.GetRenderingManager().GetChimpShaders().GetLitShader();
		shader.SetLighting(m_gameEcs->GetLightManager().GenerateLighting());

		m_ecs->GetSystems().OnRender();

		rm.SetViewport({ 0,0 }, m_engine.GetWindow().GetSize());
		rm.SetFrameBuffer();
		rm.GetRenderer().SetClearColor(0, 0, 0);
		rm.ClearColorBuffer();
	}

	void EditorScene::OnRenderUI() {
		m_ecs->GetSystems().OnRenderUI();
	}

	void EditorScene::LoadResources() {
		m_engine.GetResourceManager().GetModels().Depend(m_modelPath);
	}

	void EditorScene::UnloadResources() {
		m_engine.GetResourceManager().GetModels().Release(m_modelPath);
	}

	bool EditorScene::LoadGameECS()
	{
		const std::string path = std::string(GAME_DATA_FOLDER) + "/ecs.json";

		if (!std::filesystem::exists(path))
			return false;

		std::ifstream file(path);
		if (!file.is_open())
			return false;

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string jsonContent = buffer.str();

		m_gameEcs = Chimp::ECS::Deserialise(m_engine, jsonContent, true, false);

		return m_gameEcs != nullptr;
	}

	void EditorScene::SaveGameECS()
	{
		std::string json = m_gameEcs->Serialise();

		const std::string path = std::string(GAME_DATA_FOLDER) + "/ecs.json";

		std::filesystem::create_directories(std::filesystem::path(path).parent_path());

		std::ofstream file(path);
		if (file.is_open())
		{
			file << json;
			file.close();
		}
		else {
			GetLogger().Error(std::format("Failed to open file {}", path));
		}
	}
}