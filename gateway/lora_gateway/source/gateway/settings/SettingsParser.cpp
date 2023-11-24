
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

	if(!isSettingValid()) {
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
	settings_->setLogPath(genericSettings.at("log_path").as_string().c_str());
	settings_->setVerbose(genericSettings.at("verbose").as_bool());

	auto device_settings = jv.at("device_settings");

	settings_->setDeviceType(
			common_tools::EnumTools::stringToEnum<EDeviceType>(device_settings.at("device_type").as_string().c_str()));
	std::string device_type = device_settings.at("device_type").as_string().c_str();

	auto loraSettings = device_settings.at("lora_settings");
	settings_->setUartDevice(loraSettings.at("uart_device_path").as_string().c_str());
	settings_->setBaudRate(loraSettings.at("uart_baudrate").as_int64());

	auto output_settings = jv.at("output_settings");
	settings_->setOutputType(common_tools::EnumTools::stringToEnum<EOutputType>(output_settings.at("output_type").as_string().c_str()));

	auto csvSettings = output_settings.at("csv_settings");
	settings_->setCsvPath(csvSettings.at("csv_path").as_string().c_str());
	settings_->setNumberOfCsvEntries(csvSettings.at("number_of_entries").as_int64());
	auto mqttSettings = output_settings.at("mqtt_settings");
	settings_->setMqttBrokerAddress(mqttSettings.at("mqtt_host").as_string().c_str());
	settings_->setMqttBrokerPort(mqttSettings.at("mqtt_port").as_int64());
	settings_->setMqttTopic(mqttSettings.at("mqtt_topic").as_string().c_str());
	settings_->setMqttUsername(mqttSettings.at("mqtt_username").as_string().c_str());
	settings_->setMqttPassword(mqttSettings.at("mqtt_password").as_string().c_str());

	return true;
}

bool SettingsParser::isSettingValid() {
	bool isValid = true;

	if(settings_->getDeviceType() == EDeviceType::E_INVALID) {
		std::cout << "Device type is not valid\n";
		isValid = false;
	}

	if(settings_->getOutputType() == EOutputType::E_INVALID) {
		std::cout << "Output type is not valid\n";
		isValid = false;
	}

	return isValid;
}
}