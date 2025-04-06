#pragma once

#include "api/graphics/GameShader.h"
#include "api/graphics/shaders/shaders/lighting/Lights.h"

namespace Chimp {
	class LitShader;

	struct PointLightFS {
		Vector3f LightPosition;
		float FarPlane;
	};

	class LitPointShadowShader : public GameShader {
		friend class LitShader;
	private:
		LitPointShadowShader(Engine& engine);

	public:
		virtual ~LitPointShadowShader();

		void BeginFrame() override;
		void Render(const Mesh& mesh, const TransformMatrices& transform) override;

		void SetPointLight(const PointLight& light);

		static inline int VertexSize() {
			return 3 * sizeof(float) // position
				+ 3 * sizeof(float) // normal
				+ 2 * sizeof(float); // tex coords
		}

	private:
		PointLightMatrices m_PointLightMatrices;
		PointLightFS m_PointLightFS;
		IShaderBuffers::Index m_PointLightMatricesBufferIndex;
		IShaderBuffers::Index m_PointLightFSBufferIndex;
	};
}