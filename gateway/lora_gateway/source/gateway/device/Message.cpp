#include <gateway/device/Message.hpp>



namespace gateway::device {

const input_protocol::EDeviceType &Message::getDeviceType() const {
	return deviceType_;
}

uint32_t Message::getDeviceNumber() const {
	return deviceNumber_;
}

uint64_t Message::getCurrentTimestampMs() const {
	return currentTimestampMs_;
}

void Message::setCurrentTimestampMs(uint64_t currentTimestampMs) {
	currentTimestampMs_ = currentTimestampMs;
}

uint8_t Message::getFlags() const {
	return flags_;
}

void Message::setFlags(uint8_t flags) {
	flags_ = flags;
}
}