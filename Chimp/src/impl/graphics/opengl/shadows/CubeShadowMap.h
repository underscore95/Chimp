#pragma once

#include "graphics/shadows/IShadowMap.h"

namespace Chimp {
	class CubeShadowMap : IShadowMap {
	public:
		CubeShadowMap(unsigned int width, unsigned int height);

		void BindForWriting() const;
		void BindForReading(TextureSlot slot, const IShader& shader) const;

	private:
		static constexpr unsigned int NUM_FACES = 6;

		unsigned int m_fbo = 0;
		unsigned int m_texId = 0;
	};
}