#include <gateway/output_protocol/OutputProtocolMessage.hpp>



namespace gateway::output_protocol {

EMessageType OutputProtocolMessage::getMessageType() const {
	return messageType_;
}

uint32_t OutputProtocolMessage::getId() const {
	return id_;
}

void OutputProtocolMessage::setId(uint32_t id) {
	id_ = id;
}
}
