#include "CubeShadowMap.h"
#include "stdafx.h"
#include "Loggers.h"
#include "api/graphics/shaders/IShader.h"

namespace Chimp {
	CubeShadowMap::CubeShadowMap(unsigned int size, unsigned int numLights)
		: ICubeShadowMap(size) {
		glGenFramebuffers(1, &m_fbo);
		assert(m_fbo != 0);

		glGenTextures(1, &m_texId);
		assert(m_texId != 0);

		// Setup depth cubemap
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_texId);

		GLint filterType = GL_NEAREST; // hard shadows

		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, filterType);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		/*for (unsigned int i = 0; i < NUM_FACES; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}*/
		glTexImage3D(
			GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, size, size, 6 * numLights, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texId, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(status == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void CubeShadowMap::BindForWriting() const
	{
		glViewport(0, 0, GetWidth(), GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void CubeShadowMap::BindForReading(TextureSlot slot, const IShader& shader) const
	{
		assert(slot != 0);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_texId);

		shader.SetTextureSampler("u_CubeShadowMap", slot);
	}

	void CubeShadowMap::SetIndexToWriteTo(unsigned int index)
	{
		m_currentIndexToWriteTo = index;
	}
}