#include "api/graphics/shaders/shaders/LitShader.h"
#include "Loggers.h"
#include "Engine.h"
#include "api/graphics/shaders/shaders/lighting/Lights.h"

namespace Chimp {
	LitShader::LitShader(Engine& engine) : GameShader(
		engine, Chimp::ShaderFilePaths{
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Default.vert"),
	CHIMP_DATA_FOLDER + std::string("/Assets/Shaders/Default.frag")
		})
	{
		std::shared_ptr<Chimp::IBuffer> sceneLightingBuffer = engine.GetRenderingManager().CreateBuffer(
			sizeof(Chimp::Matrix),
			1,
			{
				Chimp::Usage::UpdateFrequency::OCCASIONAL,
				Chimp::Usage::Access::CPU_WRITE
			},
			Chimp::BindTarget::SHADER_BUFFER
		);
		m_SceneLightingBufferIndex = m_Shader->GetShaderBuffers().AddBuffer({ sceneLightingBuffer, "SceneLighting" });

		Loggers::Rendering().Info("Initialised LitShader");
	}

	LitShader::~LitShader()
	{
	}

	void LitShader::BeginFrame()
	{
		GameShader::BeginFrame();

		//Chimp::Matrix cameraMatrix = m_Camera->GetCameraMatrices().GetProjectionMatrix() * m_Camera->GetCameraMatrices().GetViewMatrix();
	//	m_Shader->SetShaderBufferSubData(m_CameraBufferId, &cameraMatrix, sizeof(Chimp::Matrix), 0);
		SceneLighting lights;
		lights.NumPointLights = 1;
		lights.PointLights[0] = {
			{ 0, 0, 0 },
			0,
			{ 1, 1, 1 },
			0
		};

		m_Shader->SetShaderBufferSubData(m_SceneLightingBufferIndex, &lights, sizeof(SceneLighting));
	}

	void LitShader::Render(const Mesh& mesh, const Matrix& transform)
	{
		for (auto& section : mesh) {
			assert(section.ElementArray->GetElementLayout().GetStride() == VertexSize()); // Does mesh vertices match what the shader expects?
		}

		GameShader::Render(mesh, transform);
	}
}