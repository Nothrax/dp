#include <gateway/endpoint/lora/UartManager.hpp>
#include <gateway/logger/Logger.hpp>

#include <wiringSerial.h>



namespace gateway::endpoint::lora {

bool UARTManager::readBuffer(uint8_t *buffer, uint32_t bufferSize) {
	//todo timeout
	while(int dataAvailable = serialDataAvail(fileDescriptor_) < bufferSize) {
		if(dataAvailable < 0) {
			logger::Logger::logError("Failed reading ttyS0");
			return false;
		}
	}
	for(int i = 0; i < bufferSize; i++) {
		buffer[i] = serialGetchar(fileDescriptor_);
	}
	return true;
}

//todo return value?
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