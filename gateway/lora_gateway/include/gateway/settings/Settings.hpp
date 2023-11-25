#pragma once

#include <string>
#include <filesystem>



namespace gateway::settings {
enum class EDeviceType {
	E_BEGIN = 0,
	E_INVALID = E_BEGIN,
	E_LORA,
	E_GENERATOR,
	E_END
};

enum class EOutputType {
	E_BEGIN = 0,
	E_INVALID = E_BEGIN,
	E_CSV,
	E_MQTT,
	E_END
};

class Settings {
public:
	[[nodiscard]] bool isVerbose() const;

	void setVerbose(bool verbose);

	[[nodiscard]] const std::string &getUartDevice() const;

	void setUartDevice(const std::string &uartDevice);

	[[nodiscard]] uint32_t getBaudRate() const;

	void setBaudRate(uint32_t baudRate);

	[[nodiscard]] EDeviceType getDeviceType() const;

	void setDeviceType(EDeviceType deviceType);

	[[nodiscard]] const std::string &getLogPath() const;

	void setLogPath(const std::string &logPath);

	[[nodiscard]] EOutputType getOutputType() const;

	void setOutputType(EOutputType outputType);

	[[nodiscard]] const std::filesystem::path &getCsvPath() const;

	void setCsvPath(const std::filesystem::path &csvPath);

	[[nodiscard]] uint32_t getNumberOfCsvEntries() const;

	void setNumberOfCsvEntries(uint32_t numberOfCsvEntries);

	[[nodiscard]] const std::string &getMqttBrokerAddress() const;

	void setMqttBrokerAddress(const std::string &mqttBrokerAddress);

	[[nodiscard]] const std::string &getMqttTopic() const;

	void setMqttTopic(const std::string &mqttTopic);

	[[nodiscard]] uint32_t getMqttBrokerPort() const;

	void setMqttBrokerPort(uint32_t mqttBrokerPort);

	[[nodiscard]] const std::string &getMqttUsername() const;

	void setMqttUsername(const std::string &mqttUsername);

	[[nodiscard]] const std::string &getMqttPassword() const;

	void setMqttPassword(const std::string &mqttPassword);

private:
	/// Print logs into console
	bool verbose_;
	/// Path to the log file
	std::string logPath_;
	/// Type of the device communication
	EDeviceType deviceType_ { EDeviceType::E_INVALID };
	/// Path to the uart device
	std::string uartDevice_;
	/// Baud rate of the uart device
	uint32_t baudRate_ { 0 };

	/// Output type
	EOutputType outputType_ { EOutputType::E_INVALID };

	/// Path to the output csv folder
	std::filesystem::path csvPath_;
	/// Number of entries in the csv file
	uint32_t numberOfCsvEntries_ { 0 };

	/// MQTT broker address
	std::string mqttBrokerAddress_;
	/// MQTT broker port
	uint32_t mqttBrokerPort_;
	/// MQTT broker port
	std::string mqttTopic_;
	/// MQTT broker port
	std::string mqttUsername_;
	/// MQTT broker port
	std::string mqttPassword_;

	//todo lora specific settings
};
}
