
#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/settings/SettingsParser.hpp>

#include <thread>
#include <iostream>



int main(int argc, char **argv) {
	using namespace gateway;
	std::thread contextThread;
	settings::SettingsParser settingsParser;
	auto context = std::make_shared<structures::GlobalContext>();

	if(!settingsParser.parseSettings(argc, argv)) {
		return EXIT_FAILURE;
	}

	context->settings = settingsParser.getSettings();

	//todo from settings
	logger::Logger::initLogger("./", true);

	try {
		Gateway worker(context);
		boost::asio::signal_set signals { context->context, SIGINT, SIGTERM, SIGHUP };
		signals.async_wait([context](auto, auto) { context->context.stop(); });
		contextThread = std::thread([context]() { context->context.run(); });

		worker.start();

	} catch(std::exception &e) {
		logger::Logger::logError(std::string("Exception occurred ") + e.what());
	} catch(...) {
		logger::Logger::logError("Unknown exception caught");
	}

	if(!context->context.stopped()) {
		context->context.stop();
	}

	contextThread.join();


	return EXIT_SUCCESS;
}


