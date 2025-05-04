#include "api/utils/TaskScheduler.h"
#include "api/Engine.h"

namespace Chimp {
	TaskScheduler::TaskScheduler(Chimp::Engine& engine) :
		m_Engine(engine)
	{
	}

	void TaskScheduler::RunDelayedTask(float delay, std::function<void()> function)
	{
		const float runTimeStamp = m_Engine.GetTimeManager().GetSecondsRunning() + delay;
		m_Tasks.emplace(runTimeStamp, function);
	}

	void TaskScheduler::Update()
	{
		const float currentSeconds = m_Engine.GetTimeManager().GetSecondsRunning();
		while (!m_Tasks.empty() && m_Tasks.top().RunTimeStamp <= currentSeconds)
		{
			m_Tasks.top().Function();
			m_Tasks.pop();
		}
	}

}