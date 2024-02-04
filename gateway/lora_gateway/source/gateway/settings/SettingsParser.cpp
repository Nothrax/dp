
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
	try {
		if(!parseConfig()) {
			std::cout << "Failed to parse json file with settings\n";
			return false;
		}
	} catch(std::exception &e) {
		std::cout << "Failed to parse json file with settings:" << e.what() <<
		"\n";
		return false;
	} catch(...) {
		std::cout << "Failed to parse json file with settings\n";
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
	po::variables_map vm;

	po::options_description optionsDescription { "Allowed options" };

	try {
		optionsDescription.add_options()
								  ("help,h", "Help")
								  ("config", po::value<std::string>(&configPath_)->required(),
								   "Path to JSON configuration file");


		po::store(po::parse_command_line(argc, argv, optionsDescription), vm);
		po::notify(vm);

		if(vm.count("help")) {
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

	settings_ = std::make_shared<Settings>();
	boost::json::value jv = boost::json::parse(json_str);

	// Extract values from the JSON object
	auto genericSettings = jv.at("generic_settings");
	settings_->setLogPath(genericSettings.at("log_path").as_string().c_str());
	settings_->setVerbose(genericSettings.at("verbose").as_bool());
	settings_->setGatewayId(genericSettings.at("gateway_id").as_string().c_str());
	settings_->setCompany(genericSettings.at("company").as_string().c_str());

	auto device_settings = jv.at("device_settings");

	auto supportedDevices = device_settings.at("supported_devices");
	std::vector<DeviceIdentification> devices;
	for(auto &device : supportedDevices.as_array()) {
		DeviceIdentification identification;
		identification.deviceType = device.at("device_type").as_int64();
		identification.deviceNumber = device.at("device_type").as_int64();
		devices.push_back(identification);
	}
	settings_->setSupportedDevices(devices);

	std::string deviceType = device_settings.at("device_communication_type").as_string().c_str();
	settings_->setDeviceType(
			common_tools::EnumTools::valueToEnum<EDeviceCommunicationType>(deviceType));

	auto loraSettings = device_settings.at("lora_settings");
	settings_->setUartDevice(loraSettings.at("uart_device_path").as_string().c_str());
	settings_->setBaudRate(loraSettings.at("uart_baudrate").as_int64());
	settings_->setM0Pin(loraSettings.at("m0_pin").as_int64());
	settings_->setM1Pin(loraSettings.at("m1_pin").as_int64());
	settings_->setLoraAddress(loraSettings.at("lora_address").as_int64());
	settings_->setLoraChannel(loraSettings.at("lora_channel").as_int64());

	auto outputSettings = jv.at("output_settings");
	std::string outputType = outputSettings.at("output_type").as_string().c_str();
	settings_->setOutputType(common_tools::EnumTools::valueToEnum<EOutputType>(outputType));

	auto csvSettings = outputSettings.at("csv_settings");
	settings_->setCsvPath(csvSettings.at("csv_path").as_string().c_str());
	settings_->setNumberOfCsvEntries(csvSettings.at("number_of_entries").as_int64());
	auto mqttSettings = outputSettings.at("mqtt_settings");
	settings_->setMqttBrokerAddress(mqttSettings.at("hostname").as_string().c_str());
	settings_->setMqttBrokerPort(mqttSettings.at("port").as_int64());
	settings_->setMqttUsername(mqttSettings.at("username").as_string().c_str());
	settings_->setMqttPassword(mqttSettings.at("password").as_string().c_str());
	settings_->setSslEnable(mqttSettings.at("ssl").as_bool());
	settings_->setClientCertificate(mqttSettings.at("client_cert").as_string().c_str());
	settings_->setClientKey(mqttSettings.at("client_key").as_string().c_str());
	settings_->setCaFile(mqttSettings.at("ca_file").as_string().c_str());

	return true;
}

bool SettingsParser::isSettingValid() {
	bool isValid = true;

	if(settings_->getDeviceType() == EDeviceCommunicationType::E_INVALID) {
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