#include "api/graphics/shaders/shaders/LitShader.h"
#include "api/graphics/shaders/shaders/lit/LitPointShadowShader.h"
#include "api/graphics/shaders/shaders/lit/LitShadowShader.h"
#include "Loggers.h"
#include "Engine.h"
#include "api/graphics/shadows/IShadowMap.h"

namespace Chimp {
	LitShader::LitShader(Engine& engine) : GameShader(
		engine, ShaderFilePaths{
			CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Lit.vert"),
			"",
			CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Lit.frag")
		}),
		m_Engine(engine),
		m_Lighting(),
		m_LightMatrices(),
		m_ShadowShader(std::unique_ptr<LitShadowShader>(new LitShadowShader(engine, m_Lighting))),
		m_PointShadowShader(std::unique_ptr<LitPointShadowShader>(new LitPointShadowShader(m_Lighting, engine))),
		m_ShadowMap(engine.GetRenderingManager().CreateShadowMap(1024, 1024, MAX_DIRECTIONAL_LIGHTS + MAX_SPOTLIGHTS)),
		m_CubeMap(engine.GetRenderingManager().CreateCubeShadowMap(1024, MAX_POINT_LIGHTS))
	{
		m_SceneLightingBufferIndex = CreateBuffer(engine, *m_Shader, sizeof(SceneLighting), "SceneLighting");
		m_LightMatricesBufferIndex = CreateBuffer(engine, *m_Shader, sizeof(LightMatrices), "LightMatrices");

		Loggers::Rendering().Info("Initialised LitShader");
	}

	LitShader::~LitShader()
	{
	}

	void LitShader::BeginFrame()
	{
		GameShader::BeginFrame();

		m_LightMatrices.NumSpotlights = m_Lighting.NumSpotlights;
		m_LightMatrices.NumDirectionalLights = m_Lighting.NumDirectionLights;

		m_Shader->SetShaderBufferSubData(m_SceneLightingBufferIndex, &m_Lighting, sizeof(SceneLighting));
		m_Shader->SetShaderBufferSubData(m_LightMatricesBufferIndex, &m_LightMatrices, sizeof(LightMatrices));
	}

	void LitShader::Render(const Mesh& mesh, const TransformMatrices& transform)
	{
		for (auto& section : mesh) {
			assert(section.ElementArray->GetElementLayout().GetStride() == VertexSize()); // Does mesh vertices match what the shader expects?
		}

		GameShader::Render(mesh, transform);
	}

	void LitShader::RenderWorld(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs)
	{
		ShadowPass(view, ecs);

		BeginFrame();
		for (auto& [transform, id, mesh] : view)
		{
			// if has health, dont render if dead
			auto health = ecs.GetComponent<HealthComponent>(id.Id);
			if (health.HasValue() && health->Health <= 0)
			{
				continue;
			}

			Render(*mesh.Mesh, { transform.GetTransformMatrix(), ToNormalMatrix(transform.GetTransformMatrix()) });
		}
	}

	void LitShader::ShadowPass(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs)
	{
		// Get camera
		bool usingCameraMatrices = m_UsingCameraMatrices;
		auto oldCamera = m_Camera;
		auto oldCameraMatrices = m_CameraMatrices;

		// Setup
		m_Lighting.IsDepthPass = true;

		m_ShadowMap->BindForReading(1, GetRawShader());
		m_CubeMap->BindForReading(2, GetRawShader());

		// Reset depth buffer
		m_ShadowMap->BindForWriting();
		m_Engine.GetRenderingManager().ClearDepthBuffer();

		// Shadow pass
		for (int i = 0; i < m_Lighting.NumSpotlights; ++i) {
			auto& spotlight = m_Lighting.Spotlights[i];

			auto matrices = spotlight.CalculateMatrices(35, m_ShadowMap->GetAspectRatio());
			auto lightMatrix = matrices.GetProjectionMatrix() * matrices.GetViewMatrix();

			SetSpotlightMatrix(i, lightMatrix);
			GetShadowShader().SetLight(i, true, matrices);

			ShadowPassPerLight(view, ecs);
		}

		for (int i = 0; i < m_Lighting.NumDirectionLights; ++i) {
			auto& light = m_Lighting.DirectionLights[i];

			auto matrices = light.CalculateMatrices(Rect{ -10,-10,20,20 });
			GetShadowShader().SetLight(i, false, matrices);

			auto lightMatrix = matrices.GetProjectionMatrix() * matrices.GetViewMatrix();
			SetDirectionalMatrix(i, lightMatrix);

			ShadowPassPerLight(view, ecs);
		}

		// Cube shadow pass
		m_CubeMap->BindForWriting();
		m_Engine.GetRenderingManager().ClearDepthBuffer();
		for (int i = 0; i < m_Lighting.NumPointLights; ++i) {
			GetPointShadowShader().SetPointLight(i);
			CubeShadowPassPerLight(view, ecs);
		}

		// Reset state
		m_Engine.GetRenderingManager().SetFrameBuffer();
		m_Engine.GetRenderingManager().SetViewport({ 0,0 }, m_Engine.GetWindow().GetSize());
		m_Engine.GetRenderingManager().ClearDepthBuffer();
		m_Engine.GetRenderingManager().ClearColorBuffer();

		m_Lighting.IsDepthPass = false;
		if (usingCameraMatrices) {
			SetCameraMatrices(oldCameraMatrices);
		}
		else {
			SetCamera(oldCamera.Get());
		}
	}

	void LitShader::SetSpotlightMatrix(int index, Matrix mat)
	{
		m_LightMatrices.Spotlights[index] = mat;
	}

	void LitShader::SetDirectionalMatrix(int index, Matrix mat)
	{
		m_LightMatrices.DirectionalLights[index] = mat;
	}

	void LitShader::ShadowPassPerLight(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs)
	{
		// Update shader
		GetShadowShader().BeginFrame();

		// Draw
		for (auto& [transform, id, mesh] : view)
		{
			// if has health, dont render if dead
			auto health = ecs.GetComponent<HealthComponent>(id.Id);
			if (health.HasValue() && health->Health <= 0)
			{
				continue;
			}

			GetShadowShader().Render(*mesh.Mesh, { transform.GetTransformMatrix(), CreateIdentityMatrix() });
		}
	}

	void LitShader::CubeShadowPassPerLight(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs)
	{
		// Update shader
		GetPointShadowShader().BeginFrame();

		// Draw
		for (auto& [transform, id, mesh] : view)
		{
			// if has health, dont render if dead
			auto health = ecs.GetComponent<HealthComponent>(id.Id);
			if (health.HasValue() && health->Health <= 0)
			{
				continue;
			}

			GetPointShadowShader().Render(*mesh.Mesh, { transform.GetTransformMatrix(), CreateIdentityMatrix() });
		}
	}
}