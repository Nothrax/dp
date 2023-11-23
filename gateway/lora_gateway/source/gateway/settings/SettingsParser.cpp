
#include <gateway/settings/SettingsParser.hpp>

namespace gateway::settings{
//todo implement
bool SettingsParser::parseSettings(int argc, char **argv){
	settings_ = std::make_shared<Settings>();
	settings_->setBaudRate(9600);
	settings_->setDeviceProtocol(EDeviceProtocol::E_LORA);
	settings_->setUartDevice("/dev/ttyS0");
	settings_->setVerbose(true);


    return true;
}

const std::shared_ptr<Settings> &SettingsParser::getSettings() const {
	return settings_;
}
}