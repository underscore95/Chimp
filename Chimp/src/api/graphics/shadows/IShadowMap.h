#pragma once

#include "api/graphics/textures/TextureSlot.h"
#include "stdafx.h"

namespace Chimp {
	class IShader;

	// TODO: Make a IRenderTexture and use that instead
	// https://www.youtube.com/watch?v=kCCsko29pv0
	class IShadowMap {
	public:
		IShadowMap(unsigned int width, unsigned int height)
			: m_width(width),
			m_height(height),
			m_aspectRatio(width / (float)height)
		{
			assert(width > 0);
			assert(height > 0);
		}

		virtual void BindForWriting() const = 0;
		virtual void BindForReading(TextureSlot slot, const IShader& shader) const = 0;

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
		inline float GetAspectRatio() const { return m_aspectRatio; }

	private:
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		float m_aspectRatio = 1;
	};
}