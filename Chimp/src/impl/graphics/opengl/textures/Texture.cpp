#include "Texture.h"
#include "../Translations.h"

namespace Chimp::GL {
	Texture::Texture(const TextureSlot slot,
		const TextureProperties& properties,
		const void* initialData) :
		ITexture(slot, properties, initialData),
		m_TextureId(0)
	{
		glGenTextures(1, &m_TextureId);
		Bind();

		// Texture properties
		// TODO: use properties struct
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// Set initial data
		const GLenum format = NumberTextureChannelsTranslation(GetProperties().NumberOfChannels);
			glTexImage2D(GL_TEXTURE_2D, 0, format, GetProperties().Width, GetProperties().Height, 0, format, GL_UNSIGNED_BYTE, initialData);
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureId);
	}

	void Texture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0 + GetSlot());
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
	}

	void Texture::Unbind() const
	{
		glActiveTexture(GL_TEXTURE0 + GetSlot());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void* Texture::GetId() const
	{
		return (void*)m_TextureId;
	}
}