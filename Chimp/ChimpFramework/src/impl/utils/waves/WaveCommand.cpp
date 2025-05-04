#include "api/utils/waves/WaveCommand.h"

namespace Chimp {
	WaveCommand::WaveCommand(float duration)
		: m_Duration(duration)
	{
	}

	float WaveCommand::GetDuration() const
	{
		return m_Duration;
	}

	LambdaWaveCommand::LambdaWaveCommand(std::function<void(Chimp::Engine&)> lambda)
		: WaveCommand(0.0f),
		m_Lambda(lambda)
	{
	}

	void LambdaWaveCommand::OnFinish(Chimp::Engine& engine)
	{
		m_Lambda(engine);
	}

	RepeatLambdaWaveCommand::RepeatLambdaWaveCommand(std::function<void(Chimp::Engine&)> lambda,
		float interval,
		unsigned int numRepeats) :
		WaveCommand(interval * numRepeats),
		m_Lambda(lambda),
		m_Interval(interval),
		m_NumRepeatsRemaining(numRepeats),
		m_SecondsSinceLastExecution(0.0f)
	{
	}

	void RepeatLambdaWaveCommand::Update(Chimp::Engine& engine)
	{
		m_SecondsSinceLastExecution += engine.GetTimeManager().GetDeltaTime();
		while (m_SecondsSinceLastExecution >= m_Interval && m_NumRepeatsRemaining > 0) {
			m_Lambda(engine);
			m_SecondsSinceLastExecution = 0;
			m_NumRepeatsRemaining--;
		}
	}

	void RepeatLambdaWaveCommand::OnFinish(Chimp::Engine& engine)
	{
		for (unsigned int i = 0; i < m_NumRepeatsRemaining; i++) {
			m_Lambda(engine);
		}
	}
}