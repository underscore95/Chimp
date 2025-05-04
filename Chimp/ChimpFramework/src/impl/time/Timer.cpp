#include "api/time/Timer.h"
#include "Loggers.h"

namespace Chimp {
	Timer::Timer() :
		m_StartTime(std::chrono::high_resolution_clock::now()),
		m_SecondsElapsed(0.0f),
		m_IsRunning(false){
	}

	Timer& Timer::Start()
	{
#ifndef NDEBUG
		if (m_IsRunning) {
			Loggers::TimeManager().Warning("Timer::Start() called on a timer that is already running.");
		}
#endif
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_IsRunning = true;
		m_SecondsElapsed = 0.0f;
		return *this;
	}

	Timer& Timer::Resume()
	{
#ifndef NDEBUG
		if (m_IsRunning) {
			Loggers::TimeManager().Warning("Timer::Resume() called on a timer that is already running.");
		}
#endif
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_IsRunning = true;
		return *this;
	}

	Timer& Timer::Stop()
	{
#ifndef NDEBUG
		if (!m_IsRunning) {
			Loggers::TimeManager().Warning("Timer::Stop() called on a timer that is not running.");
		}
#endif
		m_IsRunning = false;
		const auto stopTime = std::chrono::high_resolution_clock::now();
		m_SecondsElapsed += std::chrono::duration<float>(stopTime - m_StartTime).count();
		return *this;
	}

	float Timer::GetSecondsElapsed() const
	{
		return m_SecondsElapsed + (m_IsRunning ? std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_StartTime).count() : 0.0f);
	}

	Timer& Timer::Reset()
	{
		m_IsRunning = false;
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_SecondsElapsed = 0.0f;
		return *this;
	}
}