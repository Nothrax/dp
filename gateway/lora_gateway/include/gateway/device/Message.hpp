#pragma once

#include <gateway/input_protocol/InputProtocolMessage.hpp>

#include <boost/json.hpp>

#include <string>



namespace gateway::device {
class Message {
public:

	Message(input_protocol::EDeviceType deviceType, uint32_t deviceNumber): deviceType_(deviceType),
																			deviceNumber_(deviceNumber) {};

	/**
	 * @brief Get the Csv header of this instances device type
	 * @return csv header
	 */
	[[nodiscard]] virtual std::string getCsvHeader() const = 0;

	/**
	 * @brief Get the Csv entry of this instances device type
	 * @return csv entry
	 */
	[[nodiscard]] virtual std::string getCsvEntry() const = 0;

	/**
	 * @brief Get data of the device to publish
	 * @return
	 */
	[[nodiscard]] virtual boost::json::object getOutputProtocolEntry() const = 0;

	[[nodiscard]] const input_protocol::EDeviceType &getDeviceType() const;

	[[nodiscard]] uint32_t getDeviceNumber() const;

	[[nodiscard]] uint64_t getCurrentTimestampMs() const;

	void setCurrentTimestampMs(uint64_t currentTimestampMs);

	[[nodiscard]] uint8_t getFlags() const;

	void setFlags(uint8_t flags);

protected:
	/// Type of the device
	input_protocol::EDeviceType deviceType_;
	/// ID of the device
	uint32_t deviceNumber_ { 0 };
	/// Timestamp of the message
	uint64_t currentTimestampMs_ { 0 };
	/// Flags of the message
	uint8_t flags_ { 0 };
};
}