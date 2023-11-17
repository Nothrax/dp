
#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/settings/SettingsParser.hpp>



//todo odchytavat signal a ukoncovat komunikaci


int main(int argc, char **argv) {
    //todo inicializace, nejspis config
	std::cout << "Starting LoRa gateway\n";
    gateway::settings::SettingsParser settingsParser;

    gateway::logger::Logger::initLogger("/var/log/", true);

	auto context = std::make_shared<gateway::structures::GlobalContext>();

	gateway::Gateway worker(context);

	worker.start();
}


