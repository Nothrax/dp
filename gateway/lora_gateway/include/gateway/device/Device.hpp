#pragma once

#include <gateway/structures/DeviceMessage.hpp>
#include <gateway/common_tools/EnumTools.hpp>

#include <cstdint>
#include <string>



namespace gateway::device {
/**
 * @brief Class representing a unique device
 */
class Device {
public:
	Device(uint32_t deviceType, uint32_t deviceNumber): deviceType_(
			common_tools::EnumTools::valueToEnum<structures::EDeviceType>(deviceType)), deviceNumber_(deviceNumber) {};

	/**
	 * @brief Parse given message
	 * @param message raw message to be parsed
	 * @return true if message was parsed successfully, false otherwise
	 */
	bool parseMessage(const structures::DeviceMessage &message);

	/**
	 * @brief Get the Csv header of this instances device type
	 * @return csv header
	 */
	[[nodiscard]] std::string getCsvHeader() const;

	/**
	 * @brief Get the Csv entry of this instances device type
	 * @return csv entry
	 */
	[[nodiscard]] std::string getCsvEntry() const;

	/**
	 * @brief Get topic of the device to publish data to
	 * @return publishing topic of the device
	 */
	[[nodiscard]] std::string getMqttTopic() const;

	/**
	 * @brief Get data of the device to publish
	 * @return
	 */
	[[nodiscard]] std::string getMqttData() const;

	[[nodiscard]] structures::EDeviceType getDeviceType() const;

	[[nodiscard]] uint32_t getDeviceNumber() const;

	[[nodiscard]] bool isWrongChecksum() const;

	[[nodiscard]] bool isMessageLost() const;

private:
	/// Mask for the message lost flag
	static constexpr uint8_t MESSAGE_LOST_MASK { 0b01000000 };
	/// Mask for the wrong checksum flag
	static constexpr uint8_t WRONG_CHECKSUM_MASK { 0b10000000 };
	/// Mask for the stamp
	static constexpr uint8_t STAMP_MASK { 0b00001111 };
	/// Maximum index of the message
	static constexpr uint32_t MAX_MESSAGE_INDEX { 15 };
	/// Type of the device
	structures::EDeviceType deviceType_ { structures::EDeviceType::E_INVALID };
	/// ID of the device
	uint32_t deviceNumber_ { 0 };
	/// Current device message
	structures::DeviceMessage message_;
	/// Stamp of the last received message
	int64_t lastStamp_ { -1 };
	/// Is the checksum on last parsed message wrong
	bool isWrongChecksum_ { false };
	/// Is the message lost
	bool isMessageLost_ { false };
};
}