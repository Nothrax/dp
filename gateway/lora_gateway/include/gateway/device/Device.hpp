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
	 * @brief Get the Csv entry of this instance
	 * @return csv entry
	 */
	[[nodiscard]] std::string getCsvEntry() const;

	/**
	 * @brief Get the device number
	 * @return device number
	 */
	[[nodiscard]] uint32_t getDeviceNumber() const;

	/**
	 * @brief Get the device type
	 * @return device type
	 */
	[[nodiscard]] structures::EDeviceType getDeviceType() const;

	/**
	 * @brief Get the last parsed message checksum status
	 * @return true, if the checksum on last parsed message was wrong, false otherwise
	 */
	[[nodiscard]] bool isWrongChecksum() const;

	/**
	 * @brief Get the last parsed message lost status
	 * @return true, if the message was lost between current and last message, false otherwise
	 */
	[[nodiscard]] bool isMessageLost() const;
	//todo get mqtt data + topic

private:
	/// Mask for the message lost flag
	static constexpr uint8_t messageLostMask_ { 0b01000000 };
	/// Mask for the wrong checksum flag
	static constexpr uint8_t wrongChecksumMask_ { 0b10000000 };
	/// Mask for the stamp
	static constexpr uint8_t stampMask_ { 0b00001111 };
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