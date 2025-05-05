#pragma once

#include "api/audio/ISound.h"
#include "PlayingAudio.h"

namespace Chimp {
	class Sound : public ISound {
	public:
		Sound(std::unique_ptr<IAudioImporter::AudioData> data) :
			ISound(std::move(data)) {
			alGenBuffers(1, &m_Id);
			CHECK_AL_ERROR();
			alBufferData(m_Id, m_Data->Format, m_Data->Data.data(), m_Data->Data.size() * sizeof(short), m_Data->Frequency);
			CHECK_AL_ERROR();
		}

		virtual ~Sound() {
			for (auto& audio : m_PlayingAudios) {
				audio->Destroy();
			}

			alDeleteBuffers(1, &m_Id);
			CHECK_AL_ERROR();
		}

		std::shared_ptr<IPlayingAudio> Play(
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float gain,
			bool looping
		) override {
			m_PlayingAudios.push_back(std::make_shared<PlayingAudio>(m_Id, position, velocity, pitch, gain, looping));
			return m_PlayingAudios.back();
		}

		bool IsAnySoundsPlaying() const override {
			return !m_PlayingAudios.empty();
		}

		void Update() override {
			std::erase_if(m_PlayingAudios, [](const std::shared_ptr<IPlayingAudio>& audio) {
				return audio.use_count() <= 1 && !audio->IsPlaying();
				});
		}

	private:
		ALuint m_Id;
	};
}