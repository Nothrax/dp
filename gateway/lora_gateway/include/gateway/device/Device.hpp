#pragma once

#include <gateway/structures/DeviceMessage.hpp>
#include <gateway/common_tools/EnumTools.hpp>

#include <boost/json.hpp>

#include <cstdint>
#include <string>



namespace gateway::device {

/**
 * @brief Class representing a unique device
 */
class Device {
public:
	Device(structures::EDeviceType deviceType, uint32_t deviceNumber): deviceType_(deviceType),
																	   deviceNumber_(deviceNumber) {};

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
	[[nodiscard]] virtual std::string getCsvHeader() const = 0;

	/**
	 * @brief Get the Csv entry of this instances device type
	 * @return csv entry
	 */
	[[nodiscard]]virtual std::string getCsvEntry() const = 0;

	/**
	 * @brief Get data of the device to publish
	 * @return
	 */
	[[nodiscard]]virtual boost::json::object getOutputProtocolEntry() const = 0;

	/**
	 * @brief Get the type of the device
	 * @return device type
	 */
	[[nodiscard]] structures::EDeviceType getDeviceType() const;

	/**
	 * @brief Get the number of the device
	 * @return device number
	 */
	[[nodiscard]] uint32_t getDeviceNumber() const;

	/**
	 * @brief Had last message wrong checksum?
	 * @return true for yes, false for no
	 */
	[[nodiscard]] bool isWrongChecksum() const;

	/**
	 * @brief Was message before last lost?
	 * @return true for yes, false for no
	 */
	[[nodiscard]] bool isMessageLost() const;

protected:
	/// Mask for the message lost flag
	static constexpr uint8_t MESSAGE_LOST_MASK { 0b01000000 };
	/// Mask for the wrong checksum flag
	static constexpr uint8_t WRONG_CHECKSUM_MASK { 0b10000000 };
	/// Mask for the stamp
	static constexpr uint8_t STAMP_MASK { 0b00001111 };
	/// Maximum index of the message
	static constexpr uint32_t MAX_MESSAGE_INDEX { 15 };
	/// Type of the device
	structures::EDeviceType deviceType_;
	/// ID of the device
	uint32_t deviceNumber_ { 0 };
	/// Stamp of the last received message
	int64_t lastStamp_ { -1 };
	/// Is the checksum on last parsed message wrong
	bool isWrongChecksum_ { false };
	/// Is the message lost
	bool isMessageLost_ { false };
	uint64_t currentTimestampMs_{0};
	uint8_t flags_{0};

	virtual bool parseData(const structures::DeviceMessage &message) = 0;
};
}