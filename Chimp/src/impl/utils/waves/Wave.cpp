#include "api/utils/waves/Wave.h"
#include "Loggers.h"

namespace Chimp {
	bool Wave::Update(Engine& engine)
	{
		if (m_Commands.empty()) {
			return true;
		}

		{
			auto& command = m_Commands.front();
			command->Update(engine);
			m_CommandElapsedTime += engine.GetTimeManager().GetDeltaTime();

			if (m_CommandElapsedTime < command->GetDuration()) {
				return false;
			}
			command->OnFinish(engine);
		}

		// Next command
		m_CommandElapsedTime = 0.0f;
		while (true) {
			m_Commands.pop();
			if (m_Commands.empty()) {
				return true;
			}
			auto& command = m_Commands.front();
			if (command->GetDuration() > 0.0f) {
				break;
			}
			else {
				command->OnFinish(engine);
			}
		}
		return false;
	}

	void Wave::Delay(float seconds)
	{
		if (seconds <= 0.0f) [[unlikely]] {
			Loggers::WaveManager().Warning("Attempted to delay wave by " + std::to_string(seconds) + " seconds.");
			return;
		}
		m_Commands.push(std::make_unique<DelayWaveCommand>(seconds));
	}

	void Wave::RunFunction(std::function<void(Engine&)> function)
	{
		m_Commands.push(std::make_unique<LambdaWaveCommand>(function));
	}

	void Wave::RunRepeatingFunction(std::function<void(Engine&)> function, float interval, unsigned int repeats)
	{
		m_Commands.push(std::make_unique<RepeatLambdaWaveCommand>(function, interval, repeats));
	}
}