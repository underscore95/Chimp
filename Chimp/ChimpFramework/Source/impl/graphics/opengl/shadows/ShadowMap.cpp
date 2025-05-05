#include "ShadowMap.h"
#include "stdafx.h"
#include "Loggers.h"
#include "api/graphics/shaders/IShader.h"

namespace Chimp {
	ShadowMap::ShadowMap(unsigned int width, unsigned int height, int numLights)
		: IShadowMap(width, height) {

		glGenFramebuffers(1, &m_fbo);
		assert(m_fbo != 0);

		glGenTextures(1, &m_texId);
		assert(m_texId != 0);

		// Setup depth buffer
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, width, height, numLights, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		GLint filterType = GL_NEAREST; // hard shadows

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filterType);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texId, 0);

		// Disable read/writes to the color buffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(status == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void ShadowMap::BindForWriting() const
	{
		glViewport(0, 0, GetWidth(), GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void ShadowMap::BindForReading(TextureSlot slot, const IShader& shader) const
	{
		assert(slot != 0);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);

		shader.SetTextureSampler("u_ShadowMaps", slot);
	}
}