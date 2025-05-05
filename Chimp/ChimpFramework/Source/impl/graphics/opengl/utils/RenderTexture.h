#pragma once

#include "graphics/utils/IRenderTexture.h"

namespace Chimp {
	class RenderTexture : IRenderTexture {
	public:
		RenderTexture(unsigned int width, unsigned int height);

		void BindForWriting() const;
		void BindForReading(TextureSlot slot, const IShader& shader, const std::string& samplerName) const;

		virtual ImTextureID GetImGuiImageHandle() const { return (ImTextureID)(intptr_t)m_texId; }

	private:
		unsigned int m_fbo = 0;
		unsigned int m_texId = 0;
		unsigned int m_depthRbo = 0;
	};
}