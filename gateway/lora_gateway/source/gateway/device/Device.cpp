#include <gateway/device/Device.hpp>
#include <gateway/common_tools/MessageTools.hpp>
#include <gateway/logger/Logger.hpp>



namespace gateway::device {

bool Device::parseCommonHeader(const input_protocol::InputProtocolMessage &message) {
	if(common_tools::EnumTools::valueToEnum<input_protocol::EDeviceType, uint32_t>(message.deviceType) != deviceType_) {
		logger::Logger::logError("Incorrect device type");
		return false;
	}
	if(message.deviceNumber != deviceNumber_) {
		logger::Logger::logError("Incorrect device number");
		return false;
	}

	validateChecksum(message);

	uint8_t stamp = (message.flags & STAMP_MASK);
	validateMessageStamp(stamp);

	parseDeviceSpecificFlags(message.flags);

	return isCorrectChecksum_;
}

input_protocol::EDeviceType Device::getDeviceType() const {
	return deviceType_;
}

uint32_t Device::getDeviceNumber() const {
	return deviceNumber_;
}

bool Device::isCorrectChecksum() const {
	return isCorrectChecksum_;
}

bool Device::isMessageLost() const {
	return isMessageLost_;
}

void Device::validateMessageStamp(uint8_t currentStamp) {
	if(lastStamp_ != -1) {
		uint32_t nextStamp = lastStamp_ + 1;
		if(nextStamp > MAX_MESSAGE_INDEX) {
			nextStamp = 0;
		}
		if(nextStamp != currentStamp) {
			logger::Logger::logError("Message got lost on LoRa radio");
			isMessageLost_ = true;
		}
	}
	lastStamp_ = currentStamp;
}

void Device::validateChecksum(const input_protocol::InputProtocolMessage &message) {
	uint32_t checkSum = common_tools::MessageTools::calculateChecksum((uint8_t *)&message,
																	  sizeof(input_protocol::InputProtocolMessage) -
																	  sizeof(uint32_t));
	isCorrectChecksum_ = (checkSum == message.checkSum);
}

}
