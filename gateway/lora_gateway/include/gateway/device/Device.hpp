#pragma once

#include <gateway/input_protocol/InputProtocolMessage.hpp>
#include <gateway/common_tools/EnumTools.hpp>
#include <gateway/device/Entry.hpp>

#include <cstdint>
#include <string>
#include <memory>



namespace gateway::device {

/**
 * @brief Class representing a unique device
 */
class Device {
public:
	Device(input_protocol::EDeviceType deviceType, uint32_t deviceNumber): deviceType_(deviceType),
																		   deviceNumber_(deviceNumber) {};

	/**
	 * @brief Parse given message
	 * @param message input protocol raw binary message to be parsed
	 * @return true if message was parsed successfully, false otherwise
	 */
	virtual std::shared_ptr<Entry> parseMessage(const input_protocol::InputProtocolMessage &message) = 0;

	[[nodiscard]] uint32_t getDeviceNumber() const;

	[[nodiscard]] input_protocol::EDeviceType getDeviceType() const;

	[[nodiscard]] bool isCorrectChecksum() const;

	[[nodiscard]] bool isMessageLost() const;

protected:
	static_assert(static_cast<int>(input_protocol::EDeviceType::E_WINE_CELLAR) == 1, "Device type mismatch");
	static_assert(static_cast<int>(input_protocol::EDeviceType::E_BEE_SCALE) == 2, "Device type mismatch");
	/// Mask for the message lost flag
	static constexpr uint8_t MESSAGE_LOST_MASK { 0b01000000 };
	/// Mask for the wrong checksum flag
	static constexpr uint8_t WRONG_CHECKSUM_MASK { 0b10000000 };
	/// Mask for the stamp
	static constexpr uint8_t STAMP_MASK { 0b00001111 };
	/// Maximum index of the message
	static constexpr uint32_t MAX_MESSAGE_INDEX { 15 };
	/// Type of the device
	input_protocol::EDeviceType deviceType_;
	/// ID of the device
	uint32_t deviceNumber_ { 0 };
	/// Most current message
	/// Stamp of the last received message
	int64_t lastStamp_ { -1 };
	/// Is the checksum on last parsed message wrong
	bool isCorrectChecksum_ { false };
	/// Is the message lost
	bool isMessageLost_ { false };

	bool parseCommonHeader(const input_protocol::InputProtocolMessage &message);

	void validateMessageStamp(uint8_t currentStamp);

	void validateChecksum(const input_protocol::InputProtocolMessage &message);

	virtual bool parseDeviceSpecificFlags(const uint8_t &flags) = 0;
};
}