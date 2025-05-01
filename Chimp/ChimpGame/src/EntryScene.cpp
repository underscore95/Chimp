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

	m_Camera.SetPosition(Vector3f{ 10, 6, 10 });
	m_Camera.SetForwardVector(m_Camera.GetPosition() * -1.0f); // Look at 0 0 0

	auto& renderingManager = m_Engine.GetRenderingManager();
	renderingManager.GetRenderer().SetClearColor(0.1, 0.1, 0.5);
	renderingManager.GetChimpShaders().GetLitShader().SetCamera(m_Camera);

	auto ent = m_ECS.CreateEntity();
	m_ECS.SetComponent(ent, TransformComponent{ {0,-3,-2},{},{1,1,1} });
	m_ECS.SetComponent(ent, EntityIdComponent{ ent });
	m_ECS.SetComponent(ent, MeshComponent{ &m_TestMesh });

	ent = m_ECS.CreateEntity();
	m_ECS.SetComponent(ent, TransformComponent{ {0,-5,0}, { 0.0f,ToRadians(90.0f),0.0f},{100,100,1} });
	m_ECS.SetComponent(ent, EntityIdComponent{ ent });
	m_ECS.SetComponent(ent, MeshComponent{ &m_TestSprite });

	// Setup lighting
	auto& shader = m_Engine.GetRenderingManager().GetChimpShaders().GetLitShader();

	SceneLighting& lights = shader.GetLighting();
	lights.Ambient = { 0.25, 0.25, 0.25 };
	lights.NumPointLights = 1;

	lights.PointLights[0] = {
		{ 0, 10, 0 }, // Position
		0,
		{ 1,0,0 }, // Colour
		0,
		{ 1.0f, 0.02f, 0.0f }, // Attenuation
		0
	};

	lights.NumDirectionLights = 0;
	lights.DirectionLights[0] = {
	{ 0.5f, -1.0f, 0.0f }, // Direction
	0,
	{ 0,0,1 }, // Colour
	0
	};
	lights.DirectionLights[0].Direction = VectorNormalized(lights.DirectionLights[0].Direction);

	lights.NumSpotlights = 0;
	lights.Spotlights[0] =
	{
		{ 0, -1, 0 }, // Direction
		0,
		{ 0, 10, 0 }, // Position
		0,
		{0,1,0}, // Color
		0,
		{1.0f,0.0f,0.0f}, // Attenuation
		Cos(35), // Cutoff angle
	};
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

	auto view = m_ECS.GetEntitiesWithComponents<TransformComponent, EntityIdComponent, MeshComponent>();

	shader.RenderWorld(view, m_ECS);
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