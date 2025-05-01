#pragma once

#include "graphics/shadows/IShadowMap.h"

namespace Chimp {
	class ShadowMap : IShadowMap {
	public:
		ShadowMap(unsigned int width, unsigned int height, int numLights);

		void BindForWriting() const;
		void BindForReading(TextureSlot slot, const IShader& shader) const;

	private:
		unsigned int m_fbo = 0;
		unsigned int m_texId = 0;
	};
}