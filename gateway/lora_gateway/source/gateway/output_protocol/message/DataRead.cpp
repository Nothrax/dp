#include <gateway/output_protocol/message/DataRead.hpp>

namespace gateway::output_protocol::message {

void DataRead::parse(const boost::json::object &message) {
	messageType_ = output_protocol::EMessageType::E_DATA_READ;
	id_ = message.at("id").as_int64();
	deviceType_ = static_cast<input_protocol::EDeviceType>(message.at("device_type").as_int64());
	deviceNumber_ = message.at("device_id").as_int64();
}

input_protocol::EDeviceType DataRead::getDeviceType() const {
	return deviceType_;
}

uint32_t DataRead::getDeviceNumber() const {
	return deviceNumber_;
}
}
