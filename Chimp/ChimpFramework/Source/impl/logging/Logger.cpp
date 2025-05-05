#include "api/logging/Logger.h"

namespace Chimp {
	Logger::Logger(const std::string& name, LogLevel level)
		: m_Name(name), m_Level(level), m_LogToFile(true), m_LogToConsole(true), m_Disabled(false) {
		try {
			// Create logs directory if it doesn't exist
			std::filesystem::path path = "logs";
			if (!std::filesystem::exists(path)) {
				std::filesystem::create_directory(path);
			}
			path /= m_Name;
			if (!std::filesystem::exists(path)) {
				std::filesystem::create_directory(path);
			}

			// Create file name
			std::stringstream fileName;
			const auto now = std::chrono::system_clock::now();
			const auto nowTime = std::chrono::system_clock::to_time_t(now);
			fileName << "log_" << std::put_time(std::localtime(&nowTime), "%F_%H-%M-%S");

			// Make sure it's unique
			auto createPath = [&]() { return path / (fileName.str() + ".log"); };
			while (std::filesystem::exists(createPath())) {
				fileName << "_" << std::rand();
			}
			path = createPath();

			// Open file
			m_File = std::make_unique<std::ofstream>(path);
			if (!m_File->is_open()) {
				std::cerr << "Failed to open log file at: " << path.string() << std::endl;
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			std::cerr << "Filesystem error: " << e.what() << std::endl;

		}
		catch (const std::exception& e) {
			std::cerr << "Error in Logger initialization: " << e.what() << std::endl;
		}
	}

	Logger::~Logger()
	{
		if (m_File) {
			m_File->close();
		}
	}

	void Logger::Info(const std::string& message)const
	{
		Log(LogLevel::INFO, message);
	}

	void Logger::Info(const char* message)const
	{
		Info(std::string(message));
	}

	void Logger::Info(const std::stringstream& message)const
	{
		Info(message.str());
	}

	void Logger::Warning(const std::string& message)const
	{
		Log(LogLevel::WARNING, message);
	}

	void Logger::Warning(const char* message)const
	{
		Warning(std::string(message));
	}

	void Logger::Warning(const std::stringstream& message)const
	{
		Warning(message.str());
	}

	void Logger::Error(const std::string& message)const
	{
		Log(LogLevel::SEVERE, message);
	}

	void Logger::Error(const char* message)const
	{
		Error(std::string(message));
	}

	void Logger::Error(const std::stringstream& message)const
	{
		Error(message.str());
	}

	void Logger::Log(LogLevel level, const std::string& message)const
	{
		if (level < m_Level || m_Disabled) return;

		std::stringstream ss;
		// Name
		ss << "[" << m_Name << "] ";

		// Log level
		switch (level)
		{
		case LogLevel::INFO:
			ss << "[INFO] ";
			break;
		case LogLevel::WARNING:
			ss << "[WARNING] ";
			break;
		case LogLevel::SEVERE:
			ss << "[ERROR] ";
			break;
		}

		// Timestamp
		auto now = std::chrono::system_clock::now();
		auto nowTime = std::chrono::system_clock::to_time_t(now);
		ss << "[" << std::put_time(std::localtime(&nowTime), "%F %T") << "] ";

		// Message
		ss << message << std::endl;

		std::string finalLog = ss.str();

		// Print to console
		if (m_LogToConsole) {
			if (level < LogLevel::SEVERE) std::cout << finalLog;
			else std::cerr << finalLog;
		}

		// Save to file
		if (m_LogToFile) {
			*m_File << finalLog;
		}
	}

	void Logger::SetLogToFile(bool logToFile)
	{
		m_LogToFile = logToFile;
	}

	void Logger::SetLogToConsole(bool logToConsole)
	{
		m_LogToConsole = logToConsole;
	}

	void Logger::SetDisabled(bool disabled)
	{
		m_Disabled = disabled;
	}

	void Logger::SetEnabled(bool enabled)
	{
		m_Disabled = !enabled;
	}

	void Logger::Enable()
	{
		m_Disabled = false;
	}

	void Logger::Disable()
	{
		m_Disabled = true;
	}
}