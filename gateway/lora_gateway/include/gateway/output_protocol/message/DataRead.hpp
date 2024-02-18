#pragma once


#include <gateway/common_tools/OutputProtocolTools.hpp>
#include <gateway/output_protocol/OutputProtocolMessage.hpp>



namespace gateway::output_protocol::message {
class DataRead: public OutputProtocolMessage {
public:
	explicit DataRead(): OutputProtocolMessage(EMessageType::E_DATA_READ) {};

	void parse(const boost::json::object &message);

	[[nodiscard]] input_protocol::EDeviceType getDeviceType() const;

	[[nodiscard]] uint32_t getDeviceNumber() const;

private:
	input_protocol::EDeviceType deviceType_ { input_protocol::EDeviceType::E_INVALID };
	uint32_t deviceNumber_ { 0 };
};
}