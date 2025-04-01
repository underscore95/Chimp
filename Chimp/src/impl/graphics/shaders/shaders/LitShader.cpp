#include "api/graphics/shaders/shaders/LitShader.h"
#include "Loggers.h"
#include "Engine.h"

namespace Chimp {
	IShaderBuffers::Index CreateBuffer(Engine& engine, IShader& shader, size_t size, std::string_view name) {
		std::shared_ptr<IBuffer> buffer = engine.GetRenderingManager().CreateBuffer(
			sizeof(SceneLighting),
			1,
			{
				Usage::UpdateFrequency::OCCASIONAL,
				Usage::Access::CPU_WRITE
			},
			BindTarget::SHADER_BUFFER
		);
		return shader.GetShaderBuffers().AddBuffer({ buffer, name.data()});
	}

	LitShader::LitShader(Engine& engine) : GameShader(
		engine, ShaderFilePaths{
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Default.vert"),
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Default.frag")
		}),
		m_lighting(),
		m_lightMatrices()
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
}