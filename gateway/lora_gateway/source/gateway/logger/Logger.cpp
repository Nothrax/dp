#include <gateway/logger/Logger.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <memory>
#include <iostream>



namespace gateway::logger {

const std::string Logger::LOGGER_NAME = "gateway_logger";

bool Logger::initLogger(const std::filesystem::path &logPath, bool verbose) {
	try {
		std::vector<spdlog::sink_ptr> sinks;

		if(verbose) {
			sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
		}

		auto errorLogPath = logPath/"gateway_error.log";
		auto errorFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
				errorLogPath.string(), LOG_SIZE, 1, false);
		errorFileSink->set_level(spdlog::level::warn);
		sinks.push_back(errorFileSink);

		auto infoLogPath = logPath/"gateway_info.log";
		auto infoFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(infoLogPath.string(), LOG_SIZE, 1,
																				   false);
		sinks.push_back(infoFileSink);

		auto readerLogger = std::make_shared<spdlog::logger>(LOGGER_NAME, begin(sinks), end(sinks));

		readerLogger->flush_on(spdlog::level::warn);

		spdlog::register_logger(readerLogger);
	}
	catch(const spdlog::spdlog_ex &ex) {
		std::cerr << "Log initialization failed: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

void Logger::logInfo(const std::string &log) {
	auto logger = spdlog::get(LOGGER_NAME);
	logger->info(log);
}

void Logger::logWarning(const std::string &log) {
	auto logger = spdlog::get(LOGGER_NAME);
	logger->warn(log);
}

void Logger::logError(const std::string &log) {
	auto logger = spdlog::get(LOGGER_NAME);
	logger->error(log);
}
}
