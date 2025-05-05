#include "api/audio/sfx/SoundEffect.h"
#include "api/resources/ResourceContainer.h"
#include "api/resources/ResourceDependency.h"

namespace Chimp {
	const SoundEffect::Heuristic SoundEffect::RandomHeuristic = [](std::vector<SoundEffect::SoundType>& sounds, Random& random) -> size_t
		{
			return random.GetRandomIntExclusive(0, sounds.size());
		};

	const SoundEffect::Heuristic SoundEffect::LeastPlayingHeuristic = [](std::vector<SoundEffect::SoundType>& sounds, Random& random) -> size_t
		{
			std::vector<size_t> leastPlaying;
			int min = std::numeric_limits<int>::max();
			for (size_t i = 0; i < sounds.size(); i++)
			{
				if (sounds[i].NumberPlaying < min)
				{
					min = sounds[i].NumberPlaying;
				}
			}
			for (size_t i = 0; i < sounds.size(); i++)
			{
				if (sounds[i].NumberPlaying == min)
				{
					leastPlaying.push_back(i);
				}
			}
			return leastPlaying[random.GetRandomIntExclusive(0, leastPlaying.size())];
		};

	SoundEffect::SoundEffect(Random& random,
		ResourceContainer<SoundResourcePath, ISound>& soundResourceContainer,
		const SoundEffectSettings& settings,
		const Heuristic& heuristic) :
		m_Random(random),
		m_SoundResourceContainer(soundResourceContainer),
		m_Settings(settings),
		m_Heuristic(heuristic)
	{
		for (const auto& soundFile : m_Settings.GetSoundFiles())
		{
			m_Sounds.emplace_back(m_SoundResourceContainer, soundFile);
		}
	}

	void SoundEffect::Play(Vector3f position, Vector3f velocity, float volume)
	{
		if (m_PlayingSounds.size() >= m_Settings.GetMaxSimultaneousSounds())
		{
			return;
		}

		const float pitch = m_Random.GetRandomFloatInclusive(m_Settings.GetMinPitch(), m_Settings.GetMaxPitch());

		m_PlayingSounds.push_back(m_Sounds[m_Heuristic(m_Sounds, m_Random)].Resource.GetResource().Play(position, velocity, pitch, volume, false));
	}

	void SoundEffect::PlayForce(Vector3f position, Vector3f velocity, float volume)
	{
		while (m_PlayingSounds.size() >= m_Settings.GetMaxSimultaneousSounds())
		{
			m_PlayingSounds.front()->Stop();
			m_PlayingSounds.erase(m_PlayingSounds.begin());
		}
		Play(position, velocity, volume);
	}

	void SoundEffect::Update()
	{
		for (size_t i = 0; i < m_PlayingSounds.size(); i++)
		{
			if (!m_PlayingSounds[i]->IsPlaying())
			{
				m_PlayingSounds.erase(m_PlayingSounds.begin() + i);
				i--;
			}
		}
	}

	SoundEffect::SoundType::SoundType(ResourceContainer<SoundResourcePath, ISound>& container, const std::string& sound) :
		Resource(container, sound),
		NumberPlaying(0)
	{
	}
}