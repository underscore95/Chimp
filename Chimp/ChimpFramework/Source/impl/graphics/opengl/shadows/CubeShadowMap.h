#pragma once

#include "graphics/shadows/ICubeShadowMap.h"

namespace Chimp {
	class CubeShadowMap : public ICubeShadowMap {
	public:
		CubeShadowMap(unsigned int size, unsigned int numLights);

		void BindForWriting() const;
		void BindForReading(TextureSlot slot, const IShader& shader) const;

	private:
		static constexpr unsigned int NUM_FACES = 6;

		unsigned int m_fbo = 0;
		unsigned int m_texId = 0;
	};
}