
#include <gateway/settings/SettingsParser.hpp>
#include <gateway/common_tools/EnumTools.hpp>

#include <boost/program_options.hpp>
#include <boost/json.hpp>

#include <iostream>
#include <fstream>



namespace gateway::settings {
bool SettingsParser::parseSettings(int argc, char **argv) {
	if(!parseCmdArguments(argc, argv)) {
		return false;
	}
	if(!parseConfig()) {
		std::cout << "Failed to pars json file with arguments\n";
		return false;
	}

	if(!areSettingsValid()) {
		std::cout << "Settings are not valid\n";
		return false;
	}

	return true;
}

const std::shared_ptr<Settings> &SettingsParser::getSettings() const {
	return settings_;
}

bool SettingsParser::parseCmdArguments(int argc, char **argv) {
	namespace po = boost::program_options;
	po::variables_map variablesMap;

	po::options_description optionsDescription { "Allowed options" };

	try {
		optionsDescription.add_options()
								  ("help,h", "Help")
								  ("config", po::value<std::string>(&configPath_)->required(),
								   "Path to JSON configuration file");


		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, optionsDescription), vm);
		po::notify(vm);

		if(variablesMap.count("help")) {
			std::cout << optionsDescription << std::endl;
			return false;
		}

	}
	catch(const boost::program_options::required_option &e) {
		std::cerr << "Wrong startup arguments: " << e.what() << std::endl;
		std::cerr << "See help for more information: -h, --help" << std::endl;
		return false;
	}
	catch(std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return false;
	}
	catch(...) {
		std::cerr << "Unknown error!" << std::endl;
		return false;
	}

	return true;

}

bool SettingsParser::parseConfig() {
	if(!std::filesystem::exists(configPath_)) {
		std::cout << "Config file \"" << configPath_ << "\" does not exist\n";
		return false;
	}

	std::ifstream ifs(configPath_);
	std::string json_str((std::istreambuf_iterator<char>(ifs)),
						 (std::istreambuf_iterator<char>()));

	//todo fill the settings
	settings_ = std::make_shared<Settings>();
	boost::json::value jv = boost::json::parse(json_str);

	// Extract values from the JSON object
	auto genericSettings = jv.at("generic_settings");
	bool verbose = genericSettings.at("verbose").as_bool();
	settings_->setLogPath(genericSettings.at("log_path").as_string().c_str());

	auto device_settings = jv.at("device_settings");
	//todo device type translation
	settings_->setDeviceProtocol(common_tools::EnumTools::stringToEnum<EDeviceType>(device_settings.at("device_type").as_string().c_str()));
	std::string device_type = device_settings.at("device_type").as_string().c_str();

	auto loraSettings = device_settings.at("lora_settings");
	std::string uart_device_path = loraSettings.at("uart_device_path").as_string().c_str();
	int uart_baudrate = loraSettings.at("uart_baudrate").as_int64();

	auto output_settings = jv.at("output_settings");
	std::string output_type = output_settings.at("output_type").as_string().c_str();

	auto csvSettings = output_settings.at("csv_settings");
	std::string csv_path = csvSettings.at("csv_path").as_string().c_str();
	int number_of_entries = csvSettings.at("number_of_entries").as_int64();

	auto mqttSettings = output_settings.at("mqtt_settings");
	std::string mqtt_host = mqttSettings.at("mqtt_host").as_string().c_str();
	int mqtt_port = mqttSettings.at("mqtt_port").as_int64();
	std::string mqtt_topic = mqttSettings.at("mqtt_topic").as_string().c_str();
	std::string mqtt_username = mqttSettings.at("mqtt_username").as_string().c_str();
	std::string mqtt_password = mqttSettings.at("mqtt_password").as_string().c_str();

	// Print the extracted values
	std::cout << "verbose: " << verbose << std::endl;
	std::cout << "log_path: " << log_path << std::endl;
	std::cout << "device_type: " << device_type << std::endl;
	std::cout << "uart_device_path: " << uart_device_path << std::endl;
	std::cout << "uart_baudrate: " << uart_baudrate << std::endl;
	std::cout << "output_type: " << output_type << std::endl;
	std::cout << "csv_path: " << csv_path << std::endl;
	std::cout << "number_of_entries: " << number_of_entries << std::endl;
	std::cout << "mqtt_host: " << mqtt_host << std::endl;
	std::cout << "mqtt_port: " << mqtt_port << std::endl;
	std::cout << "mqtt_topic: " << mqtt_topic << std::endl;
	std::cout << "mqtt_username: " << mqtt_username << std::endl;
	std::cout << "mqtt_password: " << mqtt_password << std::endl;
	return true;
}

bool SettingsParser::areSettingsValid() {
	//todo
	return false;
}
}