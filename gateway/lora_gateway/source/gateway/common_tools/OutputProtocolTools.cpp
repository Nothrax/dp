#include <gateway/common_tools/OutputProtocolTools.hpp>
#include <gateway/logger/Logger.hpp>

#include <boost/json.hpp>

#include <iostream>



namespace gateway::common_tools {

std::string
OutputProtocolTools::generateDataMessage(const std::vector<std::shared_ptr<device::Message>> &messages, uint32_t id, bool storedMessages,
										 input_protocol::EDeviceType deviceType, uint32_t deviceNumber) {
	boost::json::object dataMessage;
	dataMessage["device_type"] = static_cast<int>(deviceType);
	dataMessage["id"] = id;
	dataMessage["stored_messages"] = storedMessages;
	dataMessage["device_number"] = deviceNumber;

	boost::json::array messages_array;
	for(const auto &message: messages) {
		if(message->getDeviceNumber() != deviceNumber || message->getDeviceType() != deviceType) {
			logger::Logger::logError(
					"Cannot create DATA message, device number or type does not correspond to other messages.");
			return "";
		}
		messages_array.push_back(message->getOutputProtocolEntry());
	}
	dataMessage["messages"] = messages_array;

	return boost::json::serialize(dataMessage);
}
}