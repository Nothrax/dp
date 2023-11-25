
#include <gateway/settings/Settings.hpp>



namespace gateway::settings {

bool Settings::isVerbose() const {
	return verbose_;
}

void Settings::setVerbose(bool verbose) {
	verbose_ = verbose;
}

const std::string &Settings::getUartDevice() const {
	return uartDevice_;
}

void Settings::setUartDevice(const std::string &uartDevice) {
	uartDevice_ = uartDevice;
}

unsigned int Settings::getBaudRate() const {
	return baudRate_;
}

void Settings::setBaudRate(unsigned int baudRate) {
	baudRate_ = baudRate;
}

EDeviceCommunicationType Settings::getDeviceType() const {
	return deviceType_;
}

void Settings::setDeviceType(EDeviceCommunicationType deviceProtocol) {
	deviceType_ = deviceProtocol;
}

const std::string &Settings::getLogPath() const {
	return logPath_;
}

void Settings::setLogPath(const std::string &logPath) {
	logPath_ = logPath;
}

EOutputType Settings::getOutputType() const {
	return outputType_;
}

void Settings::setOutputType(EOutputType outputType) {
	outputType_ = outputType;
}

const std::filesystem::path &Settings::getCsvPath() const {
	return csvPath_;
}

void Settings::setCsvPath(const std::filesystem::path &csvPath) {
	csvPath_ = csvPath;
}

uint32_t Settings::getNumberOfCsvEntries() const {
	return numberOfCsvEntries_;
}

void Settings::setNumberOfCsvEntries(uint32_t numberOfCsvEntries) {
	numberOfCsvEntries_ = numberOfCsvEntries;
}

const std::string &Settings::getMqttBrokerAddress() const {
	return mqttBrokerAddress_;
}

void Settings::setMqttBrokerAddress(const std::string &mqttBrokerAddress) {
	mqttBrokerAddress_ = mqttBrokerAddress;
}

const std::string &Settings::getMqttTopic() const {
	return mqttTopic_;
}

void Settings::setMqttTopic(const std::string &mqttTopic) {
	mqttTopic_ = mqttTopic;
}

uint32_t Settings::getMqttBrokerPort() const {
	return mqttBrokerPort_;
}

void Settings::setMqttBrokerPort(uint32_t mqttBrokerPort) {
	mqttBrokerPort_ = mqttBrokerPort;
}

const std::string &Settings::getMqttUsername() const {
	return mqttUsername_;
}

void Settings::setMqttUsername(const std::string &mqttUsername) {
	mqttUsername_ = mqttUsername;
}

const std::string &Settings::getMqttPassword() const {
	return mqttPassword_;
}

void Settings::setMqttPassword(const std::string &mqttPassword) {
	mqttPassword_ = mqttPassword;
}
}