#pragma once

#include "stdafx.h"
#include "api/resources/ResourceContainer.h"

namespace Chimp {
	class ITexture;
	class Engine;

	class TextureResourceContainer : public ResourceContainer<TextureResourcePath, ITexture> {
	public:
		TextureResourceContainer(Engine& engine);

	private:
		[[nodiscard]] Resource<ITexture> LoadResource(Engine& engine, const TextureResourcePath& path);
	};
}