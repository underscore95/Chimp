#include "api/graphics/GameShader.h"
#include "api/ecs/Components.h"
#include "api/Engine.h"

namespace Chimp {
	GameShader::GameShader(Engine& engine,
		const ShaderFilePaths& shaderFilePaths,
		const std::string& cameraBufferName,
		const std::string& modelBufferName) :
		m_Engine(engine),
		m_ShaderFilePaths(shaderFilePaths),
		m_Camera(&engine.GetRenderingManager().GetRenderer().GetDefaultCamera()) {

		auto& renderingManager = m_Engine.GetRenderingManager();

		// COMPILE SHADER

		m_Shader = m_Engine.GetResourceManager().GetShaders().ImmediateDepend(m_ShaderFilePaths);

		// CAMERA BUFFER
		std::shared_ptr<Chimp::IBuffer> cameraBuffer = renderingManager.CreateBuffer(
			sizeof(Chimp::Matrix),
			1,
			{
				Chimp::Usage::UpdateFrequency::OCCASIONAL,
				Chimp::Usage::Access::CPU_WRITE
			},
			Chimp::BindTarget::SHADER_BUFFER
		);
		m_CameraBufferId = m_Shader->GetShaderBuffers().AddBuffer({ cameraBuffer, cameraBufferName });

		// MODEL BUFFER
		std::shared_ptr<Chimp::IBuffer> modelBuffer = renderingManager.CreateBuffer(
			sizeof(Chimp::Matrix),
			1,
			{
				Chimp::Usage::UpdateFrequency::OFTEN,
				Chimp::Usage::Access::CPU_WRITE
			},
			Chimp::BindTarget::SHADER_BUFFER
		);
		m_ModelBufferId = m_Shader->GetShaderBuffers().AddBuffer({ modelBuffer, modelBufferName });
	}

	GameShader::~GameShader() {
		m_Engine.GetResourceManager().GetShaders().Release(m_ShaderFilePaths);
	}

	void GameShader::SetDefaultCamera() {
		m_Camera = &m_Engine.GetRenderingManager().GetRenderer().GetDefaultCamera();
	}

	void GameShader::SetCamera(ICamera& camera) {
		m_Camera = &camera;
	}

	void GameShader::BeginFrame() {
		m_IsFrameBegun = true;

		// Update camera
		Chimp::Matrix cameraMatrix = m_Camera->GetCameraMatrices().GetProjectionMatrix() * m_Camera->GetCameraMatrices().GetViewMatrix();
		m_Shader->SetShaderBufferSubData(m_CameraBufferId, &cameraMatrix, sizeof(Chimp::Matrix), 0);
	}

	void GameShader::Render(const Mesh& mesh, const Matrix& transform) {
		assert(m_IsFrameBegun);

		// Send transform
		m_Shader->SetShaderBufferSubData(m_ModelBufferId, &transform, sizeof(Chimp::Matrix), 0);

		for (const auto& section : mesh)
		{
			// Send the texture
			assert(section.Texture); // This shader doesn't support no texture
			m_Shader->SetTextureSampler(
				"u_ActiveTexture",
				section.Texture->GetResource()
			);

			// Draw the section
			m_Engine.GetRenderingManager().GetRenderer().Draw(section, *m_Shader);
		}
	}
}