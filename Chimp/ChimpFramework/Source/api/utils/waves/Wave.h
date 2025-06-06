#pragma once

#include "WaveCommand.h"

namespace Chimp {

	// A wave represents a single wave and is part of a wave manager
	// Each wave is a sub class of this class and calls "wave creation" functions in its constructor (e.g delay, run function)
	// You should probably make a subclass for your game and add your own wave creation commands by relying on the run function commands. (e.g spawn enemy, play sound)
	// A wave contains a collection of wave commands. Each wave command is executed in order and the wave is finished when all commands are finished.
	class WaveManager;
	class Wave {
		friend class WaveManager;
	protected:
		Wave() {}
	public:
		virtual ~Wave() = default;

		// Update, return true if wave is finished
		bool Update(Engine& engine);

	protected:
		// WAVE CREATION
		// Delay the next command by a certain amount of time
		void Delay(float seconds);
		// Run a function once
		void RunFunction(std::function<void(Engine&)> function);
		// Run a function every interval for a certain amount of repeats
		void RunRepeatingFunction(std::function<void(Engine&)> function, float interval, unsigned int repeats);
		// END WAVE CREATION

	protected:
		std::queue<std::unique_ptr<WaveCommand>> m_Commands;

	private:
		float m_CommandElapsedTime = 0.0f;
	};
}