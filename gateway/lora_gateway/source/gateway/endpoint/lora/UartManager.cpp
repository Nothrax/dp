#include <gateway/endpoint/lora/UartManager.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/common_tools/TimeTools.hpp>

#include <wiringSerial.h>

#include <thread>



namespace gateway::endpoint::lora {

bool UARTManager::readBuffer(std::vector<uint8_t>& buffer, uint32_t timeoutMs) const {
	auto startTime = common_tools::TimeTools::getUnixTimestampMs();
	while(int dataAvailable = serialDataAvail(fileDescriptor_) < buffer.capacity()) {
		if(dataAvailable < 0) {
			logger::Logger::logError("Failed reading ttyS0");
			return false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		///check for timeout
		if(common_tools::TimeTools::getUnixTimestampMs() - startTime > timeoutMs) {
			return false;
		}
	}
	for(auto& byte : buffer) {
		byte = serialGetchar(fileDescriptor_);
	}

	return true;
}

bool UARTManager::sendBuffer(std::vector<uint8_t>& buffer) const {
	for(const auto& byte : buffer) {
		serialPutchar(fileDescriptor_, byte);
	}
	return true;
}

UARTManager::~UARTManager() {
	serialClose(fileDescriptor_);
}

bool UARTManager::startUart(const std::string &device, int baudRate) {
	fileDescriptor_ = serialOpen(device.c_str(), baudRate);
	if(fileDescriptor_ < 0) {
		logger::Logger::logError("Cannot open device " + device);
		return false;
	}
	/// FLush the buffer
	while(serialDataAvail(fileDescriptor_) > 0) {
		serialGetchar(fileDescriptor_);
	}
	logger::Logger::logInfo("Uart device " + device + " successfully opened");
	return true;
}
}