
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

int32_t Settings::getM0Pin() const {
	return m0Pin_;
}

void Settings::setM0Pin(int32_t m0Pin) {
	m0Pin_ = m0Pin;
}

int32_t Settings::getM1Pin() const {
	return m1Pin_;
}

void Settings::setM1Pin(int32_t m1Pin) {
	m1Pin_ = m1Pin;
}

uint16_t Settings::getLoraAddress() const {
	return loraAddress_;
}

void Settings::setLoraAddress(uint16_t loraAddress) {
	loraAddress_ = loraAddress;
}

uint8_t Settings::getLoraChannel() const {
	return loraChannel_;
}

void Settings::setLoraChannel(uint8_t loraChannel) {
	loraChannel_ = loraChannel;
}

bool Settings::isSslEnable() const {
	return sslEnable_;
}

void Settings::setSslEnable(bool sslEnable) {
	sslEnable_ = sslEnable;
}

const std::filesystem::path &Settings::getClientKey() const {
	return clientKey_;
}

void Settings::setClientKey(const std::filesystem::path &clientKey) {
	clientKey_ = clientKey;
}

const std::filesystem::path &Settings::getClientCertificate() const {
	return clientCertificate_;
}

void Settings::setClientCertificate(const std::filesystem::path &clientCertificate) {
	clientCertificate_ = clientCertificate;
}

const std::string &Settings::getUser() const {
	return user_;
}

void Settings::setUser(const std::string &user) {
	user_ = user;
}

const std::string &Settings::getGatewayId() const {
	return gatewayId_;
}

void Settings::setGatewayId(const std::string &gatewayId) {
	gatewayId_ = gatewayId;
}

const std::filesystem::path &Settings::getCaFile() const {
	return caFile_;
}

void Settings::setCaFile(const std::filesystem::path &caFile) {
	caFile_ = caFile;
}
}