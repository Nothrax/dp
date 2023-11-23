
#include <gateway/settings/Settings.hpp>

namespace gateway::settings{

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

EDeviceType Settings::getDeviceProtocol() const {
	return deviceProtocol_;
}

void Settings::setDeviceProtocol(EDeviceType deviceProtocol) {
	deviceProtocol_ = deviceProtocol;
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
}