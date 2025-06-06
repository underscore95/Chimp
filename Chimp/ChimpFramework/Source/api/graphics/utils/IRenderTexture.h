#pragma once

#include "api/graphics/textures/TextureSlot.h"
#include "stdafx.h"

namespace Chimp {
	class IShader;

	// TODO make this inherit ITexture
	class IRenderTexture {
	public:
		IRenderTexture(unsigned int width, unsigned int height)
			: m_width(width),
			m_height(height),
			m_aspectRatio(width / (float)height)
		{
			assert(width > 0);
			assert(height > 0);
		}

		virtual void BindForWriting() const = 0;
		virtual void BindForReading(TextureSlot slot, const IShader& shader, const std::string& samplerName) const = 0;

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
		inline float GetAspectRatio() const { return m_aspectRatio; }

		virtual ImTextureID GetImGuiImageHandle() const = 0;

	private:
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		float m_aspectRatio = 1;
	};
}