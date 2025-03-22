#include "MenuScene.h"
#include "Logger.h"

MenuScene::MenuScene(Chimp::Engine& engine)
	: m_Engine(engine),
	m_Controller(m_Camera, m_Engine.GetWindow().GetInputManager())
{
	LoadResources();
}

MenuScene::~MenuScene()
{
	UnloadResources();
}

void MenuScene::OnInit()
{
	m_TestMesh = m_Engine.GetResourceManager().GetModels().Get(GAME_DATA_FOLDER + std::string("/Assets/Models/teapot.obj"));
}

void MenuScene::OnActivate(std::unique_ptr<Chimp::Scene> previousScene)
{
	m_Engine.GetWindow().SetTitle("Chimp Demo");
	m_Engine.GetWindow().SetSize({ 1280, 720 });
	m_Engine.GetWindow().SetResizable(true);

	auto& renderingManager = m_Engine.GetRenderingManager();
	renderingManager.GetChimpShaders().GetLitShader().SetCamera(m_Camera);

	auto ent = m_ECS.CreateEntity();
	m_ECS.SetComponent(ent, Chimp::TransformComponent{ {0,0,-10},{},{1,1,1} });
	m_ECS.SetComponent(ent, Chimp::EntityIdComponent{ ent });
	m_ECS.SetComponent(ent, Chimp::MeshComponent{ &m_Engine.GetResourceManager().GetModels().Get(m_ModelPath) });
}

void MenuScene::OnDeactivate()
{
}

void MenuScene::OnUpdate()
{
	m_Controller.OnUpdate(m_Engine.GetTimeManager().GetDeltaTime());
}

void MenuScene::OnRender()
{
	using namespace Chimp;
	auto& shader = m_Engine.GetRenderingManager().GetChimpShaders().GetLitShader();

	shader.BeginFrame();

	auto view = m_ECS.GetEntitiesWithComponents<TransformComponent, EntityIdComponent, MeshComponent>();
	for (auto& [transform, id, mesh] : view)
	{
		// if has health, dont render if dead
		auto health = m_ECS.GetComponent<HealthComponent>(id.Id);
		if (health.HasValue() && health->Health <= 0)
		{
			continue;
		}

		shader.Render(*mesh.Mesh, transform.GetTransformMatrix());
	}
}

void MenuScene::OnRenderUI()
{

}

void MenuScene::LoadResources()
{
	m_Engine.GetResourceManager().GetModels().ImportSettings.IncludeNormals = true;
	m_ModelPath = m_Engine.GetResourceManager().GetModels().Depend(GAME_DATA_FOLDER + std::string("/Assets/Models/teapot.obj"));
}

void MenuScene::UnloadResources()
{
}
