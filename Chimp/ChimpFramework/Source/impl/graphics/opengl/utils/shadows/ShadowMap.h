#pragma once

#include "graphics/utils/IRenderTexture.h"

namespace Chimp {
	class ShadowMap : IRenderTexture {
	public:
		ShadowMap(unsigned int width, unsigned int height, int numLights);

		void BindForWriting() const;
		void BindForReading(TextureSlot slot, const IShader& shader, const std::string& samplerName) const;

		virtual ImTextureID GetImGuiImageHandle() const { assert(false); return (ImTextureID)(intptr_t)m_texId; }

	private:
		unsigned int m_fbo = 0;
		unsigned int m_texId = 0;
	};
}