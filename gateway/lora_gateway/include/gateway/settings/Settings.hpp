#pragma once

#include <string>
#include <filesystem>
#include <vector>



namespace gateway::settings {
/**
 * @brief Enum with all possible device communication types
 */
enum class EDeviceCommunicationType {
	E_BEGIN = -1,
	E_INVALID = E_BEGIN,
	E_LORA,
	E_GENERATOR,
	E_END
};

/**
 * @brief Enum with all possible output types
 */
enum class EOutputType {
	E_BEGIN = -1,
	E_INVALID = E_BEGIN,
	E_MQTT,
	E_END
};

/**
 * @brief
 */
struct DeviceIdentification {
	int deviceType;
	int deviceNumber;
};

/**
 * @brief Class with all settings needed for the gateway
 */
class Settings {
public:
	[[nodiscard]] bool isVerbose() const;

	void setVerbose(bool verbose);

	[[nodiscard]] const std::string &getUartDevice() const;

	void setUartDevice(const std::string &uartDevice);

	[[nodiscard]] uint32_t getBaudRate() const;

	void setBaudRate(uint32_t baudRate);

	[[nodiscard]] EDeviceCommunicationType getDeviceType() const;

	void setDeviceType(EDeviceCommunicationType deviceType);

	[[nodiscard]] const std::string &getLogPath() const;

	void setLogPath(const std::string &logPath);

	[[nodiscard]] EOutputType getOutputType() const;

	void setOutputType(EOutputType outputType);

	[[nodiscard]] const std::filesystem::path &getCsvPath() const;

	void setCsvPath(const std::filesystem::path &csvPath);

	[[nodiscard]] const std::string &getMqttBrokerAddress() const;

	void setMqttBrokerAddress(const std::string &mqttBrokerAddress);

	[[nodiscard]] const std::filesystem::path &getCaFile() const;

	void setCaFile(const std::filesystem::path &caFile);

	[[nodiscard]] uint32_t getMqttBrokerPort() const;

	void setMqttBrokerPort(uint32_t mqttBrokerPort);

	[[nodiscard]] const std::string &getMqttUsername() const;

	void setMqttUsername(const std::string &mqttUsername);

	[[nodiscard]] const std::string &getMqttPassword() const;

	void setMqttPassword(const std::string &mqttPassword);

	[[nodiscard]] int32_t getM0Pin() const;

	void setM0Pin(int32_t m0Pin);

	[[nodiscard]] int32_t getM1Pin() const;

	void setM1Pin(int32_t m1Pin);

	[[nodiscard]] uint16_t getLoraAddress() const;

	void setLoraAddress(uint16_t loraAddress);

	[[nodiscard]] uint8_t getLoraChannel() const;

	void setLoraChannel(uint8_t loraChannel);

	[[nodiscard]] bool isSslEnable() const;

	void setSslEnable(bool sslEnable);

	[[nodiscard]] const std::filesystem::path &getClientKey() const;

	void setClientKey(const std::filesystem::path &clientKey);

	[[nodiscard]] const std::filesystem::path &getClientCertificate() const;

	void setClientCertificate(const std::filesystem::path &clientCertificate);

	[[nodiscard]] const std::string &getGatewayId() const;

	void setGatewayId(const std::string &gatewayId);

	[[nodiscard]] const std::vector<DeviceIdentification> &getSupportedDevices() const;

	void setSupportedDevices(const std::vector<DeviceIdentification> &supportedDevices);

	[[nodiscard]] const std::string &getCompany() const;

	void setCompany(const std::string &company);

	[[nodiscard]] int getGeneratorDeviceType() const;

	void setGeneratorDeviceType(int generatorDeviceType);

	[[nodiscard]] int getGeneratorDeviceNumber() const;

	void setGeneratorDeviceNumber(int generatorDeviceNumber);

private:
	/// Print logs into console
	bool verbose_;
	/// Path to the log file
	std::string logPath_;
	/// Type of the device communication
	EDeviceCommunicationType deviceType_ { EDeviceCommunicationType::E_INVALID };
	/// Vector of supported devices
	std::vector<DeviceIdentification> supportedDevices_;
	/// Path to the uart device
	std::string uartDevice_;
	/// Baud rate of the uart device
	uint32_t baudRate_ { 0 };
	/// ID of the gateway
	std::string gatewayId_;
	/// Owner of the gateway
	std::string company_;
	/// Output type
	EOutputType outputType_ { EOutputType::E_INVALID };

	/// For generator input set device
	int generatorDeviceType_;
	int generatorDeviceNumber_;

	/// Path to the output csv folder
	std::filesystem::path csvPath_;

	/// MQTT broker address
	std::string mqttBrokerAddress_;
	/// MQTT broker port
	uint32_t mqttBrokerPort_;
	/// MQTT broker port
	std::string mqttUsername_;
	/// MQTT broker port
	std::string mqttPassword_;
	/// SSL enabled
	bool sslEnable_ { false };
	/// Path to client certificate
	std::filesystem::path clientKey_;
	/// Path to client key
	std::filesystem::path clientCertificate_;
	/// CA file path
	std::filesystem::path caFile_;

	/// LoRa settings
	/// Index of the M0 pin
	int32_t m0Pin_;
	/// Index of the M1 pin
	int32_t m1Pin_;
	/// Address of the LoRa device
	uint16_t loraAddress_;
	/// Channel of the LoRa device
	uint8_t loraChannel_;
};
}
