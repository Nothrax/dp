#pragma once

#include <string>



namespace gateway::settings {
enum class EDeviceProtocol {
	E_BEGIN = 0,
	E_INVALID = E_BEGIN,
	E_LORA,
	E_END
};

class Settings {
public:
	bool isVerbose() const;

	void setVerbose(bool verbose);

	const std::string &getUartDevice() const;

	void setUartDevice(const std::string &uartDevice);

	uint32_t getBaudRate() const;

	void setBaudRate(uint32_t baudRate);

	EDeviceProtocol getDeviceProtocol() const;

	void setDeviceProtocol(EDeviceProtocol deviceProtocol);

private:
	bool verbose_;
	std::string uartDevice_ { "/dev/ttyS0" };
	uint32_t baudRate_ { 9600 };
	EDeviceProtocol deviceProtocol_;
	std::string logPath_;
	//todo lora specific settings
};
}
