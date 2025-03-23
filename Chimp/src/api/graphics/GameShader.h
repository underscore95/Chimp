#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/shaders/IShaderBuffers.h"
#include "api/utils/OptionalReference.h"
#include "api/ecs/ECS.h"
#include "api/ecs/components/TransformComponent.h"

namespace Chimp {
	class Engine;
	class ICamera;
	class Mesh;

	// Simple abstraction around Chimp's renderer which is specific to the shader (so it sends correct textures, uniforms, etc)
	class GameShader {
	public:
		GameShader(Engine& engine,
			const ShaderFilePaths& shaderFilePaths,
			const std::string& cameraBufferName = "Camera",
			const std::string& modelBufferName = "Model");
		virtual ~GameShader();

		// Change the camera
		void SetDefaultCamera();
		void SetCamera(ICamera& camera);

		// Should be called at the beginning of each frame
		virtual void BeginFrame();

		// Render a mesh
		virtual void Render(const Mesh& mesh, const TransformMatrices& transform);

	protected:
		OptionalReference<Chimp::IShader> m_Shader;

	private:
		Engine& m_Engine;
		ShaderFilePaths m_ShaderFilePaths;
		Reference<ICamera> m_Camera;
		ShaderBufferId m_CameraBufferId;
		ShaderBufferId m_ModelBufferId;
		bool m_IsFrameBegun = false;
	};
}