#pragma once

#include "api/resources/containers/SoundEffectResourceContainer.h"
#include "api/Engine.h"
#include "api/audio/sfx/SoundEffect.h"
#include "api/audio/ISound.h"

namespace Chimp {
	SoundEffectResourceContainer::SoundEffectResourceContainer(Engine& engine,
		ResourceContainer<SoundResourcePath, ISound>& soundResourceContainer) :
		ResourceContainer([&](const SoundEffectResourcePath& path) { return LoadResource(engine, soundResourceContainer, path); })
	{
	}

	Resource<SoundEffect> SoundEffectResourceContainer::LoadResource(Engine& engine,
		ResourceContainer<SoundResourcePath, ISound>& soundResourceContainer,
		const SoundEffectResourcePath& path)
	{
		assert(!path.empty());

		auto settings = UNIQUE_PTR_CAST_AND_MOVE(SoundEffectSettings, engine.GetYAMLSerialiser().ReadFromFile(std::filesystem::path(path)));
		settings->MakeFilesRelativeTo(path);
		return Resource<SoundEffect>(std::make_unique<SoundEffect>(engine.GetRandom(), soundResourceContainer, *settings));
	}

	void SoundEffectResourceContainer::Update()
	{
		for (auto& resource : m_Resources)
		{
			resource.second.Data->Update();
		}
	}
}