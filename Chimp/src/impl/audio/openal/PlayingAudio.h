#pragma once

#include "api/audio/IPlayingAudio.h"
#include "Audio.h"

namespace Chimp {
	class PlayingAudio : public IPlayingAudio {
	public:
		PlayingAudio(
			ALuint soundId,
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float volume,
			bool looping 
		) :
			IPlayingAudio(position, velocity, pitch, volume, looping),
			m_SoundId(soundId)
		{
			alGenSources(1, &m_SourceId);
			CHECK_AL_ERROR();

			alSourcei(m_SourceId, AL_BUFFER, m_SoundId); 
			CHECK_AL_ERROR();

			SetPosition(position);
			SetVelocity(velocity);
			SetPitch(pitch);
			SetVolume(volume);
			SetLooping(looping);

			alSourcePlay(m_SourceId);
			CHECK_AL_ERROR();
		}

		~PlayingAudio() {
			Destroy();
		}

		void Destroy() override {
			if (IsValid()) {
				alSourceStop(m_SourceId);
				CHECK_AL_ERROR();
				alDeleteSources(1, &m_SourceId);
				CHECK_AL_ERROR();
				m_IsValid = false;
			}
		}

		bool IsPlaying() const override {
			if (!IsValid()) return false;

			ALint state;
			alGetSourcei(m_SourceId, AL_SOURCE_STATE, &state);
			CHECK_AL_ERROR();
			return state == AL_PLAYING;
		}

		bool IsValid() const override {
			return m_IsValid && m_SoundId != 0 && m_SourceId != 0;
		}

		void Pause() override {
			alSourcePause(m_SourceId);
			CHECK_AL_ERROR();
		}

		void Resume() override {
			alSourcePlay(m_SourceId);
			CHECK_AL_ERROR();
		}

		void Stop() override {
			alSourceStop(m_SourceId);
			CHECK_AL_ERROR();
		}

		void SetPitch(float pitch) override {
			m_Pitch = pitch;
#ifndef NDEBUG
			if (pitch < 0.5f || pitch > 2.0f) {
				Loggers::Audio().Warning("Pitch is out of range! Pitch: " + std::to_string(pitch));
			}
#endif
			alSourcef(m_SourceId, AL_PITCH, pitch);
			CHECK_AL_ERROR();
		}

		void SetVolume(float volume) override {
			m_Volume = volume;
#ifndef NDEBUG
			if (volume < 0.0f) {
				Loggers::Audio().Warning("Volume is out of range! volume: " + std::to_string(volume));
			}
#endif
			alSourcef(m_SourceId, AL_GAIN, volume);
			CHECK_AL_ERROR();
		}

		void SetPosition(const Vector3f& position) override {
			m_Position = position;
			alSource3f(m_SourceId, AL_POSITION, position.x, position.y, position.z);
			CHECK_AL_ERROR();
		}

		void SetVelocity(const Vector3f& velocity) override {
			m_Velocity = velocity;
			alSource3f(m_SourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
			CHECK_AL_ERROR();
		}

		void SetLooping(bool looping) override {
			m_Looping = looping;
			alSourcei(m_SourceId, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
			CHECK_AL_ERROR();
		}
		
	private:
		ALuint m_SoundId;
		ALuint m_SourceId;
	};
}