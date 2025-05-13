#pragma once

#include "stdafx.h"
#include "api/resources/ResourceContainer.h"
#include "api/audio/AudioManager.h"

namespace Chimp {
	class Engine;
	class ResourceManager;

	class SoundResourceContainer : public ResourceContainer<SoundResourcePath, ISound> {
		friend class ResourceManager;
	public:
		SoundResourceContainer(Engine& engine);

	private:
		[[nodiscard]] Resource<ISound> LoadResource(Engine& engine, const SoundResourcePath& path);

		void Update();
	};
}