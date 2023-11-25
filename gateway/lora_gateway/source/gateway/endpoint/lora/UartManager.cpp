#include <gateway/endpoint/lora/UartManager.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/common_tools/TimeTools.hpp>

#include <wiringSerial.h>

#include <thread>



namespace gateway::endpoint::lora {

bool UARTManager::readBuffer(uint8_t *buffer, uint32_t bufferSize, uint32_t timeoutMs) {
	auto startTime = common_tools::TimeTools::getUnixTimestampMs();
	while(int dataAvailable = serialDataAvail(fileDescriptor_) < bufferSize) {
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
	for(int i = 0; i < bufferSize; i++) {
		buffer[i] = serialGetchar(fileDescriptor_);
	}
	return true;
}

bool UARTManager::sendBuffer(const uint8_t *buffer, uint32_t bufferSize) {
	for(int i = 0; i < bufferSize; i++) {
		serialPutchar(fileDescriptor_, buffer[i]);
	}
	return true;
}

UARTManager::~UARTManager() {
	serialClose(fileDescriptor_);
}

bool UARTManager::startUart(const std::string &device, uint32_t baudRate) {
	fileDescriptor_ = serialOpen(device.c_str(), baudRate);
	if(fileDescriptor_ < 0) {
		logger::Logger::logError("Cannot open device " + device);
		return false;
	}
	return true;
}
}