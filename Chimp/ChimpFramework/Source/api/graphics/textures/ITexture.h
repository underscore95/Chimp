#pragma once

#include "stdafx.h"
#include "TextureProperties.h"
#include "TextureSlot.h"
#include "api/utils/preprocessor/CopyAndMove.h"

namespace Chimp {
	class ITexture {
		DISABLE_COPY(ITexture);
	protected:
		// Represents a texture (2D buffer) on the GPU.
		ITexture(const TextureSlot slot, 
			const TextureProperties& properties,
			const void* initialData);

	public:
		virtual ~ITexture() = default;

		// Bind the texture, this will unbind any other texture that is bound to the same slot
		virtual void Bind() const = 0;

		// Unbind the texture, this will unbind any other texture that is bound to the same slot
		virtual void Unbind() const = 0;

		// Switch the slot the texture is bound to, if you call this, assume the texture is no longer bound.
		void SetSlot(const TextureSlot slot);

		// Get the slot the texture is bound to
		[[nodiscard]] TextureSlot GetSlot() const;

		// Get the id of the texture
		[[nodiscard]] virtual void* GetId() const = 0;

	protected:
		// Get the properties of the texture
		[[nodiscard]] const TextureProperties& GetProperties() const;

	private:
		TextureSlot m_Slot;
		TextureProperties m_Properties;
	};
}