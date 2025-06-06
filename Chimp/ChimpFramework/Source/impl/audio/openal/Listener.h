#pragma once

#include "api/audio/IListener.h"
#include "Audio.h"

namespace Chimp {
	class Listener : public Chimp::IListener {
	public:
		Listener() = default;
		~Listener() = default;

		void SetPosition(Vector3f position) override {
			m_Position = position;
			alListener3f(AL_POSITION, position.x, position.y, position.z);
			CHECK_AL_ERROR();
		}

		void SetVelocity(Vector3f velocity) override {
			m_Velocity = velocity;
			alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
			CHECK_AL_ERROR();
		}

		void SetOrientation(Vector3f forward, Vector3f up) override {
			m_Forward = forward;
			m_Up = up;
			float orientation[6] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
			alListenerfv(AL_ORIENTATION, orientation);
			CHECK_AL_ERROR();
		}
	};
}