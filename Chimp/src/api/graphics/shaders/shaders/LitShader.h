#pragma once

#include "api/graphics/GameShader.h"
#include "api/graphics/shaders/shaders/lighting/Lights.h"

namespace Chimp {
	class ChimpShaders;
	class LitPointShadowShader;
	class LitShadowShader;

	class LitShader : public GameShader {
		friend class ChimpShaders;
	private:
		LitShader(Engine& engine);

	public:
		virtual ~LitShader();

		SceneLighting& GetLighting() { return m_lighting; }
		void BeginFrame() override;
		void Render(const Mesh& mesh, const TransformMatrices& transform) override;

		void SetSpotlightMatrix(int index, Matrix mat);
		void SetDirectionalMatrix(int index, Matrix mat);

		static inline int VertexSize() {
			return 3 * sizeof(float) // position
				+ 3 * sizeof(float) // normal
				+ 2 * sizeof(float); // tex coords
		}

		LitPointShadowShader& GetPointShadowShader() { return *m_PointShadowShader; }
		LitShadowShader& GetShadowShader() { return *m_ShadowShader; }

	private:
		SceneLighting m_lighting;
		LightMatrices m_lightMatrices;
		std::unique_ptr<LitPointShadowShader> m_PointShadowShader;
		std::unique_ptr<LitShadowShader> m_ShadowShader;
		IShaderBuffers::Index m_SceneLightingBufferIndex;
		IShaderBuffers::Index m_LightMatricesBufferIndex;
	};
}