#pragma once

#include "stdafx.h"

namespace Chimp {
	enum class LogLevel {
		INFO = 0,
		WARNING = 1,
		SEVERE = 2
	};

	class Logger {
	public:
		Logger(const std::string& name, LogLevel level = LogLevel::INFO);
		~Logger();

		void Info(const std::string& message) const;
		void Info(const char* message) const;
		void Info(const std::stringstream &message) const;

		void Warning(const std::string& message) const;
		void Warning(const char* message) const;
		void Warning(const std::stringstream& message) const;

		void Error(const std::string& message) const;
		void Error(const char* message) const;
		void Error(const std::stringstream& message) const;

		void Log(LogLevel level, const std::string& message) const;

		void SetLogToFile(bool logToFile);
		void SetLogToConsole(bool logToConsole);
		void SetDisabled(bool disabled);
		void SetEnabled(bool enabled);
		void Enable();
		void Disable();

	private:
		std::string m_Name;
		LogLevel m_Level;
		std::unique_ptr<std::ofstream> m_File;
		bool m_LogToFile;
		bool m_LogToConsole;
		bool m_Disabled;
	};
}