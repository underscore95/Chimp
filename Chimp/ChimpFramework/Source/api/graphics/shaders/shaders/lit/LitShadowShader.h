#pragma once

#include "api/graphics/GameShader.h"
#include "api/graphics/shaders/shaders/lighting/Lights.h"

namespace Chimp {
	class LitShader;

	struct CurrentLayerBuffer {
		int CurrentLayer;
		float Padding1;
		float Padding2;
		float Padding3;
	};

	class LitShadowShader : public GameShader {
		friend class LitShader;
	private:
		LitShadowShader(Engine& engine, SceneLighting& lighting);

	public:
		virtual ~LitShadowShader();

		void BeginFrame() override;
		void Render(const Mesh& mesh, const TransformMatrices& transform) override;

		void SetLight(int index, bool isSpotlight, const CameraMatrices& matrices);

		static inline int VertexSize() {
			return 3 * sizeof(float) // position
				+ 3 * sizeof(float) // normal
				+ 2 * sizeof(float); // tex coords
		}

	private:
		CurrentLayerBuffer m_CurrentLayerBuffer;
		IShaderBuffers::Index m_CurrentLayerBufferIndex;
		SceneLighting& m_Lighting;
	};
}