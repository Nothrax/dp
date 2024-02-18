#include <gateway/device/Entry.hpp>



namespace gateway::device {

const input_protocol::EDeviceType &Entry::getDeviceType() const {
	return deviceType_;
}

uint32_t Entry::getDeviceNumber() const {
	return deviceNumber_;
}

uint64_t Entry::getCurrentTimestampMs() const {
	return currentTimestampMs_;
}

void Entry::setCurrentTimestampMs(uint64_t currentTimestampMs) {
	currentTimestampMs_ = currentTimestampMs;
}

uint8_t Entry::getFlags() const {
	return flags_;
}

void Entry::setFlags(uint8_t flags) {
	flags_ = flags;
}
}