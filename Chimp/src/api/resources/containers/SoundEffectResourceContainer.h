#pragma once

#include "stdafx.h"
#include "api/resources/ResourceContainer.h"
#include "api/audio/AudioManager.h"
#include "api/audio/sfx/SoundEffect.h"
#include "api/audio/ISound.h"

namespace Chimp {
	class Engine;
	class ResourceManager;

	class SoundEffectResourceContainer : public ResourceContainer<SoundEffectResourcePath, SoundEffect> {
		friend class ResourceManager;
	public:
		SoundEffectResourceContainer(Engine& engine, ResourceContainer<SoundResourcePath, ISound>& soundResourceContainer);

	private:
		[[nodiscard]] Resource<SoundEffect> LoadResource(Engine& engine, ResourceContainer<SoundResourcePath, ISound>& soundResourceContainer, const SoundEffectResourcePath& path);

		void Update();
	};
}