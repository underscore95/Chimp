#include "RenderTexture.h"
#include "stdafx.h"
#include "Loggers.h"
#include "api/graphics/shaders/IShader.h"

namespace Chimp {
	RenderTexture::RenderTexture(unsigned int width, unsigned int height)
		: IRenderTexture(width, height) {

		glGenFramebuffers(1, &m_fbo);
		assert(m_fbo != 0);

		// Create color texture
		glGenTextures(1, &m_texId);
		assert(m_texId != 0);
		glBindTexture(GL_TEXTURE_2D, m_texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Create depth buffer
		glGenRenderbuffers(1, &m_depthRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

		// Attach to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);

		// Specify the color buffer to draw to
		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(status == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTexture::BindForWriting() const {
		glViewport(0, 0, GetWidth(), GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void RenderTexture::BindForReading(TextureSlot slot, const IShader& shader, const std::string& samplerName) const {
		assert(slot != 0);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_texId);
		shader.SetTextureSampler(samplerName, slot);
	}
}
