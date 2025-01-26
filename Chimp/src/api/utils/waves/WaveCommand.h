#pragma once

#include "stdafx.h"
#include "api/Engine.h"

namespace Chimp {
	// A wave command is a single action that can be executed by a wave. For example spawning an enemy.
	class WaveManager;
	class WaveCommand {
		friend class WaveManager;
	protected:
		// Each wave command should only be associated with one wave
		// durationSeconds - duration in seconds that the command will be ran for
		// if duration is <= 0, update will not be executed but on finish will be.
		// if duration is > 0, update is guaranteed to be executed at least once (once per frame). On finish will also be called.
		WaveCommand(float durationSeconds);
	public:
		virtual ~WaveCommand() = default;

		// Called once per frame unless the command has no duration
		virtual void Update(Chimp::Engine& engine) = 0;

		// Called after the duration has passed
		virtual void OnFinish(Chimp::Engine& engine) {}

		// Get the duration of the command in seconds
		float GetDuration() const;

	private:
		float m_Duration;
	};

	// Delay for an amount of seconds
	class DelayWaveCommand : public WaveCommand {
	public:
		DelayWaveCommand(float durationSeconds) : WaveCommand(durationSeconds) {}

		void Update(Chimp::Engine& engine) override {}
	};

	// Run a function once, this command has no duration
	class LambdaWaveCommand : public WaveCommand {
	public:
		LambdaWaveCommand(std::function<void(Chimp::Engine&)> lambda);

		void Update(Chimp::Engine& engine) override {}

		void OnFinish(Chimp::Engine& engine) override;

	private:
		std::function<void(Chimp::Engine&)> m_Lambda;
	};

	// Repeat a function for a number of times, with a delay between each execution
	class RepeatLambdaWaveCommand : public WaveCommand {
	public:
		RepeatLambdaWaveCommand(
			std::function<void(Chimp::Engine&)> lambda,
			float interval,
			unsigned int numRepeats);

		void Update(Chimp::Engine& engine) override;

		void OnFinish(Chimp::Engine& engine) override;

	private:
		std::function<void(Chimp::Engine&)> m_Lambda;
		float m_Interval;
		unsigned int m_NumRepeats;
		float m_SecondsSinceLastExecution;
		unsigned int m_NumRepeatsRemaining;
	};
}