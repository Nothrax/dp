#pragma once

#include <string>
#include <filesystem>


namespace gateway::logger {
/**
 * @brief Logger class using spdlog as implementation
 */
class Logger {
public:
	static bool initLogger(const std::filesystem::path &logPath, bool verbose);

	//todo use std::format?
	static void logInfo(const std::string &log);

	static void logWarning(const std::string &log);

	static void logError(const std::string &log);
private:
	static const std::string LOGGER_NAME;
	/// 50MB
	static constexpr uint32_t LOG_SIZE{1024*1024*50};
};
}
