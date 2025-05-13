#pragma once

#include "api/resources/containers/SoundResourceContainer.h"
#include "api/Engine.h"

namespace Chimp {
	SoundResourceContainer::SoundResourceContainer(Engine& engine) :
		ResourceContainer([&](const SoundResourcePath& path) { return LoadResource(engine, path); })
	{
	}

	Resource<ISound> SoundResourceContainer::LoadResource(Engine& engine, const SoundResourcePath& path)
	{
		assert(!path.empty());
		return Resource<ISound>(engine.GetAudioManager().LoadSound(std::filesystem::path(path)));
	}

	void SoundResourceContainer::Update()
	{
		for (auto& resource : m_Resources)
		{
			resource.second.Data->Update();
		}
	}
}