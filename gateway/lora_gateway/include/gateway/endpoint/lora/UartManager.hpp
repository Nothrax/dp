#pragma once

#include <string>
#include <vector>


namespace gateway::endpoint::lora {
class UARTManager {
public:
	/**
	 * @brief Start UART communication
	 * @param device address of UART device
	 * @param baudRate baud rate of UART communication
	 * @return true if successful, false otherwise
	 * @return false if the initialization failed
	 */
	bool startUart(const std::string &device, int baudRate);

	/**
	 * @brief Send buffer over UART
	 * @param buffer buffer to send
	 * @param bufferSize size of the buffer
	 * @return true if successful, false otherwise
	 */
	bool sendBuffer(std::vector<uint8_t>& buffer) const;

	/**
	 * @brief read buffer from UART with timeout - wait for bufferSize to be available on UART, if timeout occurs return false and no bytes are read
	 * @param buffer buffer to read into
	 * @param bufferSize size of the buffer
	 * @param timeoutMs timeout in milliseconds
	 * @return
	 */
	bool readBuffer(std::vector<uint8_t>& buffer, uint32_t timeoutMs) const;

	/**
	 * @brief Destroy the UARTManager object
	 */
	~UARTManager();

private:
	/// File descriptor of the UART device
	int fileDescriptor_ { -1 };
};
}