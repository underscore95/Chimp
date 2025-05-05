#include "api/time/TimeManager.h"

namespace Chimp {
	constexpr float DEFAULT_DELTA_TIME = 1.0f / 60.0f; // 60 FPS

	TimeManager::TimeManager() :
		m_DeltaTime(DEFAULT_DELTA_TIME),
		m_FrameStartTime(std::chrono::steady_clock::now()) {
		m_TimeRunning.Start();
	}

	void TimeManager::Update()
	{
		const auto frameEndTime = std::chrono::steady_clock::now();
		m_DeltaTime = std::chrono::duration<float>(frameEndTime - m_FrameStartTime).count();

		m_FrameStartTime = std::chrono::steady_clock::now();
	}

	float TimeManager::GetDeltaTime() const {
		return m_DeltaTime;
	}

	float TimeManager::GetSecondsRunning() const
	{
		return m_TimeRunning.GetSecondsElapsed();
	}

	Timer TimeManager::CreateTimer() const
	{
		return Timer();
	}

	static void InsertDoubleDigitNumber(std::stringstream& ss, int number) {
		if (number < 10) {
			ss << "0";
		}
		ss << number;
	}
	std::string TimeManager::FormatTime(float seconds, const std::string& delim) const
	{
		assert(seconds >= 0);
		std::stringstream ss;
		int days = static_cast<int>(seconds / 86400);
		if (days > 0) {
			ss << days << delim;
			seconds -= days * 86400;
		}
		int hours = static_cast<int>(seconds / 3600);
		if (hours > 0) {
			InsertDoubleDigitNumber(ss, hours);
			ss << delim;
			seconds -= hours * 3600;
		}
		int minutes = static_cast<int>(seconds / 60);
		InsertDoubleDigitNumber(ss, minutes);
		ss << delim;
		seconds -= minutes * 60;
		InsertDoubleDigitNumber(ss, static_cast<int>(seconds));
		return ss.str();
	}
}