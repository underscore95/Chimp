#pragma once

#include "SoundEffectSettings.h"
#include "api/audio/IPlayingAudio.h"
#include "api/resources/ResourcePaths.h"
#include "api/utils/Random.h"
#include "api/resources/ResourceContainer.h"
#include "api/resources/ResourceDependency.h"

namespace Chimp {
	class SoundEffect {
		struct SoundType {
			SoundType(ResourceContainer<SoundResourcePath, ISound>& container, const std::string& sound);
			ResourceDependency<SoundResourcePath, ISound> Resource;
			int NumberPlaying;
		};
	public:
		typedef std::function<size_t(std::vector<SoundType>&, Random&)> Heuristic; // returns index of sound to play

		const static Heuristic RandomHeuristic; // Randomly select a sound
		const static Heuristic LeastPlayingHeuristic; // Selects least playing sound, if multiple, randomly selects one
	public:
		SoundEffect(Random& random, 
			ResourceContainer<SoundResourcePath, ISound>& soundResourceContainer,
			const SoundEffectSettings& settings,
			const Heuristic& heuristic = LeastPlayingHeuristic);

		// Play a sound
		// If force, we'll stop a playing sound if we're at the limit (if not force, we'll just not play the sound)
		void Play(Vector3f position = { 0,0,0 }, Vector3f velocity = { 0,0,0 }, float volume = 1.0f);
		void PlayForce(Vector3f position = { 0,0,0 }, Vector3f velocity = { 0,0,0 }, float volume = 1.0f);

		// Call this once per frame unless you loaded this in the resource manager
		void Update();
	private:
		Random& m_Random;
		ResourceContainer<SoundResourcePath, ISound>& m_SoundResourceContainer;
		SoundEffectSettings m_Settings;
		Heuristic m_Heuristic;
		std::vector<SoundType> m_Sounds;
		std::vector<std::shared_ptr<IPlayingAudio>> m_PlayingSounds;
	};
}