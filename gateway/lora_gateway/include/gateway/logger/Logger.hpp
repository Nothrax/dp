#pragma once

#include <string>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <fmt/format.h>



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
	template <typename ...Args>
	static void logInfo(const std::string &log, Args ...args) {
		auto logger = spdlog::get(LOGGER_NAME);
		logger->info(fmt::vformat(log, fmt::make_format_args(args...)));
	}

	/**
	 * @brief Log warning message
	 * @param log message to be logged
	 */
	template <typename ...Args>
	static void logWarning(const std::string &log, Args ...args) {
		auto logger = spdlog::get(LOGGER_NAME);
		logger->warn(fmt::vformat(log, fmt::make_format_args(args...)));
	}

	/**
	 * @brief Log error message
	 * @param log message to be logged
	 */
	template <typename ...Args>
	static void logError(const std::string &log, Args ...args) {
		auto logger = spdlog::get(LOGGER_NAME);
		logger->error(fmt::vformat(log, fmt::make_format_args(args...)));
	}

	static void destroyLogger();

private:
	/// Name of the logger - spdlog uses this to identify the logger
	static const std::string LOGGER_NAME;
	/// size of the single log file before rotation - 50MB
	static constexpr uint32_t LOG_SIZE { 1024*1024*50 };
};
}
