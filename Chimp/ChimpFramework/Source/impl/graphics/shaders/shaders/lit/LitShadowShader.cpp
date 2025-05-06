#include "api/graphics/shaders/shaders/lit/LitShadowShader.h"
#include "Loggers.h"
#include "Engine.h"

namespace Chimp {
	LitShadowShader::LitShadowShader(Engine& engine, SceneLighting& lighting)
		: GameShader(engine, ShaderFilePaths{
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/LitShadow.vert"),
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/LitShadow.geom"),
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/LitShadow.frag")
			},
			"Camera", "Model", false, true),
		m_CurrentLayerBuffer(),
		m_Lighting(lighting)
	{
		m_CurrentLayerBufferIndex = CreateBuffer(engine, *m_Shader, sizeof(CurrentLayerBuffer), "CurrentLayer");

	}

	LitShadowShader::~LitShadowShader() {
	}

	void LitShadowShader::BeginFrame() {
		GameShader::BeginFrame();

		m_Shader->SetShaderBufferSubData(m_CurrentLayerBufferIndex, &m_CurrentLayerBuffer, sizeof(CurrentLayerBuffer));
	}

	void LitShadowShader::Render(const Mesh& mesh, const TransformMatrices& transform) {
		for (auto& section : mesh) {
			assert(section.ElementArray->GetElementLayout().GetStride() == VertexSize()); // Does mesh vertices match what the shader expects?
		}

		GameShader::Render(mesh, transform);
	}

	void LitShadowShader::SetLight(int index, bool isSpotlight, const CameraMatrices& matrices) {
		m_CurrentLayerBuffer.CurrentLayer = index;
		if (isSpotlight) {
			m_CurrentLayerBuffer.CurrentLayer += m_Lighting.NumDirectionLights;
		}

		SetCameraMatrices(matrices);
	}

}