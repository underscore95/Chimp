#include "api/graphics/shaders/shaders/lit/LitPointShadowShader.h"
#include "Loggers.h"
#include "Engine.h"

namespace Chimp {
	LitPointShadowShader::LitPointShadowShader(Engine& engine) : GameShader(
		engine, ShaderFilePaths{
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/LitPointShadow.vert"),
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/LitPointShadow.geom"),
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/LitPointShadow.frag")
		},
		"Camera", "Model", true, true),
		m_PointLightMatrices(),
		m_PointLightFS()
	{
		m_PointLightMatricesBufferIndex = CreateBuffer(engine, *m_Shader, sizeof(PointLightMatrices), "PointLightMatrices");
		m_PointLightFSBufferIndex = CreateBuffer(engine, *m_Shader, sizeof(PointLightFS), "PointLightFS");
	}

	LitPointShadowShader::~LitPointShadowShader()
	{
	}

	void LitPointShadowShader::BeginFrame()
	{
		GameShader::BeginFrame();

		m_Shader->SetShaderBufferSubData(m_PointLightMatricesBufferIndex, &m_PointLightMatrices, sizeof(PointLightMatrices));
		m_Shader->SetShaderBufferSubData(m_PointLightFSBufferIndex, &m_PointLightFS, sizeof(PointLightFS));
	}

	void LitPointShadowShader::Render(const Mesh& mesh, const TransformMatrices& transform)
	{
		for (auto& section : mesh) {
			assert(section.ElementArray->GetElementLayout().GetStride() == VertexSize()); // Does mesh vertices match what the shader expects?
		}

		GameShader::Render(mesh, transform);
	}

	void LitPointShadowShader::SetPointLight(const PointLight& light)
	{
		m_PointLightMatrices = light.CalculateMatrices();
		m_PointLightFS.LightPosition = light.Position;
		m_PointLightFS.FarPlane = 20;
	}
}