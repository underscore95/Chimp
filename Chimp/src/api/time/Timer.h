#pragma once

#include "stdafx.h"

namespace Chimp {
	class Timer {
	public:
		Timer();
		~Timer() = default;

		// Start the timer, resetting the elapsed time to 0.
		// returns this timer
		Timer& Start();

		// Start the timer, but don't reset the elapsed time.
		// if hasn't been started, acts as if it was started for the first time.
		Timer& Resume();

		// Stop the timer, stopping before starting the timer will set both the start and stop time to now.
		// returns this timer
		Timer& Stop();

		// Get the time elapsed in seconds since start
		// If timer has never started, this is time elapsed since the object was constructed
		// If timer has been stopped, this is the time elapsed between start and stop
		// If timer is currently running, this is the time elapsed between start and now
		float GetSecondsElapsed() const;

		// Reset the timer, stopping (if started), and setting the elapsed time to 0.
		// returns this timer
		Timer& Reset();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		float m_SecondsElapsed;
		bool m_IsRunning;
	};
}