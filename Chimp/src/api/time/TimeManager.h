#pragma once

#include "stdafx.h"
#include "Timer.h"

namespace Chimp {
	class Engine;
	class MainLoop;
	class Timer;
	class TimeManager {
		friend class Engine;
		friend class MainLoop;
	private:
		TimeManager();

	private:
		void Update();

	public:
		[[nodiscard]] float GetDeltaTime() const;
		[[nodiscard]]  float GetSecondsRunning() const;

		// Alternative to using the constructor
		[[nodiscard]] Timer CreateTimer() const;

		// Pretty format a duration in seconds to a string, goes up to days.
		// e.g 3666 seconds -> 1:01:06 or 122 seconds -> 2:02
		[[nodiscard]] std::string FormatTime(float seconds, const std::string& delim = ":") const;

	private:
		std::chrono::steady_clock::time_point m_FrameStartTime;
		float m_DeltaTime;
		Timer m_TimeRunning;
	};
}