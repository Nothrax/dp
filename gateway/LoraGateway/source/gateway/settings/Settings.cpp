
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

EDeviceProtocol Settings::getDeviceProtocol() const {
	return deviceProtocol_;
}

void Settings::setDeviceProtocol(EDeviceProtocol deviceProtocol) {
	deviceProtocol_ = deviceProtocol;
}
}