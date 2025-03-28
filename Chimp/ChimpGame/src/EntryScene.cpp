#include "EntryScene.h"
#include "Logger.h"

using namespace Chimp;

EntryScene::EntryScene(Engine& engine)
	: m_Engine(engine),
	m_Controller(m_Camera, m_Engine.GetWindow().GetInputManager())
{
	LoadResources();
}

EntryScene::~EntryScene()
{
	UnloadResources();
}

void EntryScene::OnInit()
{
	m_TestMesh = m_Engine.GetResourceManager().GetModels().Get(m_ModelPath);
	m_TestSprite = m_Engine.GetResourceManager().GetSprites().Get(m_TexPath);
}

void EntryScene::OnActivate(std::unique_ptr<Scene> previousScene)
{
	m_Engine.GetWindow().SetTitle("Chimp Demo");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	m_Camera.SetPosition( Vector3f{20, 20, 20});
	m_Camera.SetForwardVector(m_Camera.GetPosition() * -1.0f); // Look at 0 0 0

	auto& renderingManager = m_Engine.GetRenderingManager();
	renderingManager.GetRenderer().SetClearColor(0.1, 0.1, 0.5);
	renderingManager.GetChimpShaders().GetLitShader().SetCamera(m_Camera);

	auto ent = m_ECS.CreateEntity();
	m_ECS.SetComponent(ent, TransformComponent{ {0,-3,-10},{},{1,1,1} });
	m_ECS.SetComponent(ent, EntityIdComponent{ ent });
	m_ECS.SetComponent(ent, MeshComponent{ &m_TestMesh });

	ent = m_ECS.CreateEntity();
	m_ECS.SetComponent(ent, TransformComponent{ {0,-5,0}, { 0.0f,ToRadians(90.0f),0.0f},{100,100,1}});
	m_ECS.SetComponent(ent, EntityIdComponent{ ent });
	m_ECS.SetComponent(ent, MeshComponent{ &m_TestSprite });
}

void EntryScene::OnDeactivate()
{
}

void EntryScene::OnUpdate()
{
	m_Controller.OnUpdate(m_Engine.GetTimeManager().GetDeltaTime());
}

void EntryScene::OnRender()
{
	auto& shader = m_Engine.GetRenderingManager().GetChimpShaders().GetLitShader();

	shader.BeginFrame();

	auto view = m_ECS.GetEntitiesWithComponents<TransformComponent, EntityIdComponent, MeshComponent>();
	for (auto& [transform, id, mesh] : view)
	{
		//transform.SetRotation(transform.GetRotation() +(Vector3f{0.0f, 1.0f, 0.0f} * 0.01f));
		// if has health, dont render if dead
		auto health = m_ECS.GetComponent<HealthComponent>(id.Id);
		if (health.HasValue() && health->Health <= 0)
		{
			continue;
		}

		shader.Render(*mesh.Mesh, { transform.GetTransformMatrix(), ToNormalMatrix(m_Camera.GetCameraMatrices().GetViewMatrix() * transform.GetTransformMatrix()) });
	}
}

void EntryScene::OnRenderUI()
{

}

void EntryScene::LoadResources()
{
	m_Engine.GetResourceManager().GetModels().ImportSettings.IncludeNormals = true;
	m_ModelPath = m_Engine.GetResourceManager().GetModels().Depend(GAME_DATA_FOLDER + std::string("/Assets/Models/cube.obj"));
	m_TexPath = m_Engine.GetResourceManager().GetSprites().Depend(GAME_DATA_FOLDER + std::string("/Assets/Models/gray.png"));
}

void EntryScene::UnloadResources()
{
}
