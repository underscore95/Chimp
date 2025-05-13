#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"
#include "IAudioImporter.h"

namespace Chimp {
	class IPlayingAudio;

	class ISound {
	protected:
		ISound(std::unique_ptr<IAudioImporter::AudioData> data) :
			m_Data(std::move(data))
		{
		}

	public:
		virtual ~ISound() = default;

		// Play the sound
		// position - The position of the sound
		// velocity - The velocity of the sound
		// pitch - The pitch of the sound
		// gain - The gain of the sound
		// looping - If the sound should loop
		// returns a shared ptr to the playing audio, this can be used to modify the audio while it is playing (e.g change pitch)
		[[nodiscard]] virtual std::shared_ptr<IPlayingAudio> Play(
			Vector3f position,
			Vector3f velocity = {0,0,0},
			float pitch = 1.0f,
			float gain = 1.0f,
			bool looping = false
		) = 0;

		// Check if any sounds are playing
		[[nodiscard]] virtual bool IsAnySoundsPlaying() const = 0;

		// Update the sound, must be called once per frame.
		// If the sound was loaded from the resource manager, this will be done automatically and you don't need to call it.
		virtual void Update() = 0;

		// Get duration in seconds
		[[nodiscard]] virtual float GetDurationSeconds() const {
			return m_Data->DurationSeconds;
		}

	protected:
		std::unique_ptr<IAudioImporter::AudioData> m_Data;
		std::vector<std::shared_ptr<IPlayingAudio>> m_PlayingAudios;
	};
}