#pragma once

#include <string>
#include <filesystem>


namespace gateway::logger {
/**
 * @brief Logger class using spdlog as implementation
 */
class Logger {
public:
	/**
	 * @brief Initialize logger, must be called before any logging
	 * @param logPath where to store logs
	 * @param verbose true for console logging enable
	 * @return true if successful, false otherwise
	 */
	static bool initLogger(const std::filesystem::path &logPath, bool verbose);

	/**
	 * @brief Log info message
	 * @param log message to be logged
	 */
	static void logInfo(const std::string &log);

	/**
	 * @brief Log warning message
	 * @param log message to be logged
	 */
	static void logWarning(const std::string &log);

	/**
	 * @brief Log error message
	 * @param log message to be logged
	 */
	static void logError(const std::string &log);
private:
	/// Name of the logger - spdlog uses this to identify the logger
	static const std::string LOGGER_NAME;
	/// size of the single log file before rotation - 50MB
	static constexpr uint32_t LOG_SIZE{1024*1024*50};
};
}
