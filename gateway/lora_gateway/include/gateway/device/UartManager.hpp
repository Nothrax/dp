#pragma once

#include <string>



namespace gateway::device {
class UARTManager {
public:
	bool startUart(const std::string &device, uint32_t baudRate);

	bool sendBuffer(const uint8_t *buffer, uint32_t bufferSize);

	bool readBuffer(uint8_t *buffer, uint32_t bufferSize);

	~UARTManager();

private:
	int fileDescriptor_ { -1 };
};
}