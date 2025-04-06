#include "EntryScene.h"
#include "Logger.h"

using namespace Chimp;

EntryScene::EntryScene(Engine& engine)
	: m_Engine(engine),
	m_Controller(m_Camera, m_Engine.GetWindow().GetInputManager()),
	m_ShadowMap(engine.GetRenderingManager().CreateShadowMap(1024, 1024)),
	m_CubeMap(engine.GetRenderingManager().CreateCubeShadowMap(1024, 1024))
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

	// Setup lighting
	SceneLighting& lights = shader.GetLighting();
	lights.IsDepthPass = true;

	ResetLighting(CreateIdentityMatrix(), lights);

	// Shadow pass
	assert(lights.NumSpotlights + lights.NumDirectionLights == 1);

	for (int i = 0; i < lights.NumSpotlights; ++i) {
		auto& spotlight = lights.Spotlights[i];
		assert(lights.Spotlights.size() == 1);

		auto matrices = spotlight.CalculateMatrices(35, m_ShadowMap->GetAspectRatio());
		auto lightMatrix = matrices.GetProjectionMatrix() * matrices.GetViewMatrix();
		shader.SetSpotlightMatrix(i, lightMatrix);
		shader.SetCameraMatrices(matrices);

		ShadowPass(shader, view);
	}

	for (int i = 0; i < lights.NumDirectionLights; ++i) {
		auto& light = lights.DirectionLights[i];
		assert(lights.DirectionLights.size() == 1);

		auto matrices = light.CalculateMatrices(Rect{-10,-10,20,20});
		auto lightMatrix = matrices.GetProjectionMatrix() * matrices.GetViewMatrix();
		shader.SetDirectionalMatrix(i, lightMatrix);
		shader.SetCameraMatrices(matrices);

		ShadowPass(shader, view);
	}

	// Cube shadow pass
	ResetLighting(CreateIdentityMatrix(), lights, false);
	for (int i = 0; i < lights.NumPointLights; ++i) {
		auto& pointlight = lights.PointLights[i];

		shader.GetPointShadowShader().SetPointLight(pointlight);
		CubeShadowPass(shader.GetPointShadowShader(), view);
	}

	// Render pass
	ResetLighting(m_Camera.GetCameraMatrices().GetViewMatrix(), lights);

	m_Engine.GetRenderingManager().SetFrameBuffer();
	m_Engine.GetRenderingManager().SetViewport({ 0,0 }, m_Engine.GetWindow().GetSize());
	m_Engine.GetRenderingManager().ClearDepthBuffer();
	m_Engine.GetRenderingManager().ClearColorBuffer();
	m_ShadowMap->BindForReading(1, shader.GetRawShader());

	lights.IsDepthPass = false;
	shader.SetCamera(m_Camera);

	shader.BeginFrame();
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

void EntryScene::ResetLighting(Chimp::Matrix view, Chimp::SceneLighting& lights, bool makeViewSpace)
{
	lights.Ambient = { 0.25, 0.25, 0.25 };
	lights.NumPointLights = 1;

	lights.PointLights[0] = {
		{ 0, 0, -3 }, // Position
		0,
		{ 1,0,0 }, // Colour
		0,
		{ 1.0f, 0.02f, 0.0f }, // Attenuation
		0
	};

	lights.NumDirectionLights = 1;
	lights.DirectionLights[0] = {
	{ 0.5f, -1.0f, 0.0f }, // Direction
	0,
	{ 0,0, 1 }, // Colour
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

	if (makeViewSpace) {
		for (auto& light : lights.PointLights) {
			light.Position = MatrixTransform(light.Position, view);
		}

		for (auto& light : lights.DirectionLights) {
			light.Direction *= ToNormalMatrix(view);
			light.Direction = VectorNormalized(light.Direction);
		}

		for (auto& light : lights.Spotlights) {
			light.Position = MatrixTransform(light.Position, view);
			light.Direction *= ToNormalMatrix(view);
			light.Direction = VectorNormalized(light.Direction);
		}
	}
}

void EntryScene::ShadowPass(Chimp::LitShader& shader, Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view)
{
	// Reset depth buffer
	m_ShadowMap->BindForWriting();
	m_Engine.GetRenderingManager().ClearDepthBuffer();

	// Update shader
	
	shader.BeginFrame();

	// Draw
	for (auto& [transform, id, mesh] : view)
	{
		// if has health, dont render if dead
		auto health = m_ECS.GetComponent<HealthComponent>(id.Id);
		if (health.HasValue() && health->Health <= 0)
		{
			continue;
		}

		shader.Render(*mesh.Mesh, { transform.GetTransformMatrix(), ToNormalMatrix(m_Camera.GetCameraMatrices().GetViewMatrix() * transform.GetTransformMatrix()) });
	}
}

void EntryScene::CubeShadowPass(Chimp::LitPointShadowShader& shader, Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view)
{	
	// Reset depth buffer
	m_CubeMap->BindForWriting();
	m_Engine.GetRenderingManager().ClearDepthBuffer();

	// Update shader
	shader.BeginFrame();

	// Draw
	for (auto& [transform, id, mesh] : view)
	{
		// if has health, dont render if dead
		auto health = m_ECS.GetComponent<HealthComponent>(id.Id);
		if (health.HasValue() && health->Health <= 0)
		{
			continue;
		}

		shader.Render(*mesh.Mesh, { transform.GetTransformMatrix(), ToNormalMatrix(m_Camera.GetCameraMatrices().GetViewMatrix() * transform.GetTransformMatrix()) });
	}
}
