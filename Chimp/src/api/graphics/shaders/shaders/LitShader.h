#pragma once

#include "api/graphics/GameShader.h"
#include "api/graphics/shaders/shaders/lighting/Lights.h"
#include "api/ecs/Components.h"

namespace Chimp {
	class ChimpShaders;
	class LitPointShadowShader;
	class LitShadowShader;
	class IShadowMap;

	class LitShader : public GameShader {
		friend class ChimpShaders;
	private:
		LitShader(Engine& engine);

	public:
		virtual ~LitShader();

		SceneLighting& GetLighting() { return m_lighting; }
		void BeginFrame() override;
		void Render(const Mesh& mesh, const TransformMatrices& transform) override;

		/// <summary>
		/// Render the world with shadows
		/// </summary>
		/// <param name="view>View which contains every entity that should be rendered</param>
		/// <param name="ecs>The ECS</param>
		void RenderWorld(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs);

		static inline int VertexSize() {
			return 3 * sizeof(float) // position
				+ 3 * sizeof(float) // normal
				+ 2 * sizeof(float); // tex coords
		}

	private:
		void SetSpotlightMatrix(int index, Matrix mat);
		void SetDirectionalMatrix(int index, Matrix mat);

		void ShadowPass(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs);

		LitPointShadowShader& GetPointShadowShader() { return *m_PointShadowShader; }
		LitShadowShader& GetShadowShader() { return *m_ShadowShader; }

		void ShadowPassPerLight(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs);
		void CubeShadowPassPerLight(Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view, const ECS& ecs);

	private:
		Engine& m_Engine;
		SceneLighting m_lighting;
		LightMatrices m_lightMatrices;
		std::unique_ptr<LitPointShadowShader> m_PointShadowShader;
		std::unique_ptr<LitShadowShader> m_ShadowShader;
		IShaderBuffers::Index m_SceneLightingBufferIndex;
		IShaderBuffers::Index m_LightMatricesBufferIndex;
		std::unique_ptr<Chimp::IShadowMap> m_ShadowMap;
		std::unique_ptr<Chimp::IShadowMap> m_CubeMap;
	};
}