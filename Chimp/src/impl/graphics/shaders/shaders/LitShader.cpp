#include "api/graphics/shaders/shaders/LitShader.h"
#include "api/graphics/shaders/shaders/lit/LitPointShadowShader.h"
#include "api/graphics/shaders/shaders/lit/LitShadowShader.h"
#include "Loggers.h"
#include "Engine.h"

namespace Chimp {
	LitShader::LitShader(Engine& engine) : GameShader(
		engine, ShaderFilePaths{
			CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Lit.vert"),
			"",
			CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Lit.frag")
		}),
		m_lighting(),
		m_lightMatrices(),
		m_ShadowShader(std::unique_ptr<LitShadowShader>(new LitShadowShader(engine, m_lighting))),
		m_PointShadowShader(std::unique_ptr<LitPointShadowShader>(new LitPointShadowShader(engine)))
	{
		m_SceneLightingBufferIndex = CreateBuffer(engine,*m_Shader, sizeof(SceneLighting), "SceneLighting");
		m_LightMatricesBufferIndex = CreateBuffer(engine,*m_Shader, sizeof(LightMatrices), "LightMatrices");

		Loggers::Rendering().Info("Initialised LitShader");
	}

	LitShader::~LitShader()
	{
	}

	void LitShader::BeginFrame()
	{
		GameShader::BeginFrame();

		m_lightMatrices.NumSpotlights = m_lighting.NumSpotlights;
		m_lightMatrices.NumDirectionalLights = m_lighting.NumDirectionLights;

		m_Shader->SetShaderBufferSubData(m_SceneLightingBufferIndex, &m_lighting, sizeof(SceneLighting));
		m_Shader->SetShaderBufferSubData(m_LightMatricesBufferIndex, &m_lightMatrices, sizeof(LightMatrices));
	}

	void LitShader::Render(const Mesh& mesh, const TransformMatrices& transform)
	{
		for (auto& section : mesh) {
			assert(section.ElementArray->GetElementLayout().GetStride() == VertexSize()); // Does mesh vertices match what the shader expects?
		}

		GameShader::Render(mesh, transform);
	}

	void LitShader::SetSpotlightMatrix(int index, Matrix mat)
	{
		m_lightMatrices.Spotlights[index] = mat;
	}

	void LitShader::SetDirectionalMatrix(int index, Matrix mat)
	{
		m_lightMatrices.DirectionalLights[index] = mat;
	}
}