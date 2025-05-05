#include "api/graphics/textures/ITexture.h"

namespace Chimp {
	ITexture::ITexture(const TextureSlot slot,
		const TextureProperties& properties,
		const void* initialData) :
		m_Slot(slot),
		m_Properties(properties)
	{
	}

	void ITexture::SetSlot(const TextureSlot slot)
	{
		m_Slot = slot;
	}

	TextureSlot ITexture::GetSlot() const
	{
		return m_Slot;
	}

	const TextureProperties& ITexture::GetProperties() const
	{
		return m_Properties;
	}
}