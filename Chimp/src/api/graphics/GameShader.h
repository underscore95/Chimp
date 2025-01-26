#pragma once

#include "stdafx.h"
#include "api/Engine.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/utils/OptionalReference.h"
#include "api/ecs/ECS.h"

namespace Chimp {
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
		void SetCamera(Camera& camera);

		// Should be called at the beginning of each frame
		virtual void BeginFrame();

		// Render a mesh
		virtual void Render(const Mesh& mesh, const Matrix& transform);

		// Render world, where entities with maximum z are rendered first
		virtual void RenderWorld(ECS& ecs);

	private:
		Engine& m_Engine;
		ShaderFilePaths m_ShaderFilePaths;
		Reference<Camera> m_Camera;
		ShaderBufferId m_CameraBufferId;
		ShaderBufferId m_ModelBufferId;
		OptionalReference<Chimp::IShader> m_Shader;
		bool m_IsFrameBegun = false;
	};
}