#pragma once

#include <gateway/device/Message.hpp>

#include <vector>
#include <string>
#include <memory>

//todo static check if the enum is the same as in the input protocol
enum class EMessageType{
	E_DATA = 1,
	E_DATA_ACK = 2,
	E_DATA_READ = 3,
	E_DATA_READ_RES = 4,
};

namespace gateway::common_tools {
class OutputProtocolTools {
public:
	[[nodiscard]] static std::string
	generateDataMessage(const std::vector<std::shared_ptr<device::Message>> &messages, uint32_t id, bool storedMessages,
						input_protocol::EDeviceType deviceType, uint32_t deviceNumber);
};
}