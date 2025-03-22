#pragma once

#include "api/graphics/GameShader.h"

namespace Chimp {
	class ChimpShaders;

	class LitShader : public GameShader {
		friend class ChimpShaders;
	private:
		LitShader(Engine& engine);

	public:
		virtual ~LitShader();

		void BeginFrame() override;
		void Render(const Mesh& mesh, const Matrix& transform) override;

		static inline int VertexSize() {
			return 3 * sizeof(float) // position
				+ 3 * sizeof(float) // normal
				+ 2 * sizeof(float); // tex coords
		}

	private:
		IShaderBuffers::Index m_SceneLightingBufferIndex;
	};
}