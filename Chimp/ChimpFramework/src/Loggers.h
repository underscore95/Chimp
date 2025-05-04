#pragma once

#include "api/logging/Logger.h"

namespace Chimp::Loggers {
	static Logger& Main()
	{
		static Logger logger("ChimpMain", LogLevel::INFO);
		return logger;
	}

	static Logger& Network()
	{
		static Logger logger("ChimpNetwork", LogLevel::INFO);
		return logger;
	}

	static Logger& Resources()
	{
		static Logger logger("ChimpResources", LogLevel::WARNING);
		return logger;
	}

	static Logger& Rendering()
	{
		static Logger logger("ChimpRendering", LogLevel::INFO);
		return logger;
	}

	static Logger& WaveManager()
	{
		static Logger logger("ChimpWaveManager", LogLevel::WARNING);
		return logger;
	}

	static Logger& TimeManager()
	{
		static Logger logger("ChimpTimeManager", LogLevel::WARNING);
		return logger;
	}

	static Logger& YAML()
	{
		static Logger logger("ChimpYAML", LogLevel::WARNING);
		return logger;
	}

	static Logger& Audio()
	{
		static Logger logger("ChimpAudio", LogLevel::INFO);
		return logger;
	}
}