#pragma once

#include "stdafx.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "api/graphics/shaders/IShaderBuffers.h"
#include "api/utils/OptionalReference.h"
#include "api/ecs/ECS.h"
#include "api/ecs/transform/TransformComponent.h"
#include "api/graphics/camera/CameraMatrices.h"

namespace Chimp {

	struct TransformMatrices {
		Matrix TransformMatrix;
		Matrix NormalMatrix;
	};
	static_assert(sizeof(TransformMatrices) % 16 == 0);

	class Engine;
	class ICamera;
	class Mesh;
	struct TransformComponent;
	struct MeshComponent;
	struct EntityIdComponent;
	class ECS;

	// Simple abstraction around Chimp's renderer which is specific to the shader (so it sends correct textures, uniforms, etc)
	class GameShader {
	public:
		GameShader(Engine& engine,
			const ShaderFilePaths& shaderFilePaths,
			const std::string& cameraBufferName = "Camera",
			const std::string& modelBufferName = "Model",
			bool noCamera = false,
			bool noActiveTexture = false);
		virtual ~GameShader();

		// Change the camera
		void SetDefaultCamera();
		void SetCamera(ICamera& camera);
		void SetCameraMatrices(CameraMatrices matrices);

		// Should be called at the beginning of each frame
		virtual void BeginFrame();

		// Render a mesh
		virtual void Render(const Mesh& mesh, const TransformMatrices& transform);

		// Render every entity in the world
		virtual void RenderWorld(ECS& ecs) { assert(false && "Unsupported operation on this shader."); }

		IShader& GetRawShader();

	protected:
		OptionalReference<Chimp::IShader> m_Shader;

		const CameraMatrices& GetCameraMatrices();
		const CameraMatrices* GetCameraMatricesPtr();

		static IShaderBuffers::Index CreateBuffer(Engine& engine, IShader& shader, size_t size, std::string_view name);

	protected:
		Reference<ICamera> m_Camera;
		bool m_UsingCameraMatrices;
		CameraMatrices m_CameraMatrices;

	private:
		Engine& m_Engine;
		ShaderFilePaths m_ShaderFilePaths;
		ShaderBufferId m_CameraBufferId;
		ShaderBufferId m_ModelBufferId;
		bool m_IsFrameBegun = false;
		bool m_IsNoCamera;
		bool m_IsNoActiveTexture;
	};
}