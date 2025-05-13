#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {
	class Sound;
	class IPlayingAudio {
		friend class Sound;
	protected:
		IPlayingAudio(
			Vector3f position,
			Vector3f velocity,
			float pitch,
			float volume,
			bool looping
		) :
			m_Position(position),
			m_Velocity(velocity),
			m_Pitch(pitch),
			m_Volume(volume),
			m_Looping(looping)
		{}
	public:
		virtual ~IPlayingAudio() = default;

		virtual bool IsPlaying() const = 0;
		virtual bool IsValid() const = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Stop() = 0;

		// Pitch (range 0.5 - 2.0)
		virtual void SetPitch(float pitch) = 0;

		// Gain / volume (minimum 0.0)
		virtual void SetVolume(float gain) = 0;

		virtual void SetPosition(const Vector3f& position) = 0;
		virtual void SetVelocity(const Vector3f& velocity) = 0;

		virtual void SetLooping(bool looping) = 0;

		// Adders
		void AddPitch(float pitch) {
			SetPitch(Clamp(m_Pitch + pitch, 0.5f, 2.0f));
		}

		void AddVolume(float volume) {
			SetVolume(Max(m_Volume + volume, 0.0f));
		}

		void AddVolumeThenClamp(float volume, float min, float max) {
			SetVolume(Clamp(m_Volume + volume, min, max));
		}

		// Getters
		[[nodiscard]] float GetPitch() const {
			return m_Pitch;
		}

		[[nodiscard]] float GetVolume() const {
			return m_Volume;
		}

		[[nodiscard]] const Vector3f& GetPosition() const {
			return m_Position;
		}

		[[nodiscard]] const Vector3f& GetVelocity() const {
			return m_Velocity;
		}

		[[nodiscard]] bool IsLooping() const {
			return m_Looping;
		}

	protected:
		virtual void Destroy() = 0;

	protected:
		Vector3f m_Position;
		Vector3f m_Velocity;
		float m_Pitch;
		float m_Volume;
		bool m_Looping;
		bool m_IsValid = true;
	};
}