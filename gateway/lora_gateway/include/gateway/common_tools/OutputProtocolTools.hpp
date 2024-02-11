#pragma once

#include <gateway/device/Message.hpp>
#include <gateway/output_protocol/OutputProtocolMessage.hpp>

#include <vector>
#include <string>
#include <memory>


namespace gateway::common_tools {
class OutputProtocolTools {
public:
	[[nodiscard]] static std::string
	generateDataMessage(const std::vector<std::shared_ptr<device::Message>> &messages, uint32_t id, bool storedMessages,
						input_protocol::EDeviceType deviceType, uint32_t deviceNumber);

	[[nodiscard]] static output_protocol::EMessageType getMessageType(const boost::json::object &message);
};
}