#pragma once

#include "api/audio/IAudio.h"
#include "Loggers.h"

namespace Chimp {
	class Audio : public IAudio {
	public:

		static void CheckError(const char* functionName, const char* file, int line)
		{
			ALenum error = alGetError();
			if (error != AL_NO_ERROR) [[unlikely]] {
				Loggers::Audio().Error(
					std::format("OpenAL error: {} in function: {} at file: {} line: {}",
						std::to_string(error), functionName, file, line));
				assert(false);
			}
		}

#define CHECK_AL_ERROR() Chimp::Audio::CheckError(__FUNCTION__, __FILE__, __LINE__)

		Audio() :
			IAudio(),
			m_IsValid(false)
		{
			m_Device = alcOpenDevice(nullptr);
			if (!m_Device) {
				Loggers::Audio().Error("Failed to open audio device!");
				Loggers::Audio().Error("Your device may be incapable of playing audio (try plugging some headphones in and running the application again!)");
				return;
			}

			m_Context = alcCreateContext(m_Device, nullptr);
			if (!m_Context || alcMakeContextCurrent(m_Context) == ALC_FALSE) {
				Loggers::Audio().Error("Failed to create audio context!");
				if (m_Context) {
					alcDestroyContext(m_Context);
				}
				alcCloseDevice(m_Device);
				return;
			}

			m_IsValid = true;
		}

		~Audio() {
			alcMakeContextCurrent(nullptr);

			if (m_Context) {
				alcDestroyContext(m_Context);
			}
			if (m_Device) {
				alcCloseDevice(m_Device);
			}
		}

		bool IsValid() const {
			return m_IsValid;
		}

	private:
		ALCdevice* m_Device;
		ALCcontext* m_Context;
		bool m_IsValid;
	};
}