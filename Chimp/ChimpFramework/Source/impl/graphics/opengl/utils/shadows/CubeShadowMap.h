#pragma once

#include "graphics/utils/shadows/ICubeShadowMap.h"

namespace Chimp {
	class CubeShadowMap : public ICubeShadowMap {
	public:
		CubeShadowMap(unsigned int size, unsigned int numLights);

		void BindForWriting() const;
		void BindForReading(TextureSlot slot, const IShader& shader, const std::string& samplerName) const;

		virtual ImTextureID GetImGuiImageHandle() const { assert(false); return (ImTextureID)(intptr_t)m_texId; }

	private:
		static constexpr unsigned int NUM_FACES = 6;

		unsigned int m_fbo = 0;
		unsigned int m_texId = 0;
	};
}