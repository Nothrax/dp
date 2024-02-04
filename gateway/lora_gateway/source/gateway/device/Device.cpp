#include <gateway/device/Device.hpp>
#include <gateway/common_tools/TimeTools.hpp>
#include <gateway/common_tools/MessageTools.hpp>
#include <gateway/logger/Logger.hpp>



namespace gateway::device {

bool Device::parseMessage(const structures::DeviceMessage &message) {
	if(common_tools::EnumTools::valueToEnum<structures::EDeviceType, uint32_t>(message.deviceType) != deviceType_) {
		logger::Logger::logError("Incorrect device type");
		return false;
	}
	if(message.deviceNumber != deviceNumber_) {
		logger::Logger::logError("Incorrect device number");
		return false;
	}

	isWrongChecksum_ = false;
	isMessageLost_ = false;
	currentTimestampMs_ = common_tools::TimeTools::getUnixTimestampMs();

	if(message.flags & WRONG_CHECKSUM_MASK) {
		logger::Logger::logError("Wrong check sum on 433MHZ RF radio");
		isWrongChecksum_ = true;
	}

	if(message.flags & MESSAGE_LOST_MASK) {
		logger::Logger::logError("Message got lost on 433MHZ RF radio");
		isMessageLost_ = true;
	}

	uint8_t stamp = (message.flags & STAMP_MASK);

	flags_ = message.flags;

	if(lastStamp_ != -1) {
		uint32_t nextStamp = lastStamp_ + 1;
		if(nextStamp > MAX_MESSAGE_INDEX) {
			nextStamp = 0;
		}
		if(nextStamp != stamp) {
			logger::Logger::logError("Message got lost on LoRa radio");
			isMessageLost_ = true;
		}
	}
	lastStamp_ = stamp;

	uint32_t checkSum = common_tools::MessageTools::calculateChecksum((uint8_t *)&message,
																	  sizeof(structures::DeviceMessage) -
																	  sizeof(uint32_t));
	if(checkSum != message.checkSum) {
		isWrongChecksum_ = true;
		logger::Logger::logError("Wrong checksum");
	}

	parseData(message);

	return !isWrongChecksum_;
}

structures::EDeviceType Device::getDeviceType() const {
	return deviceType_;
}

uint32_t Device::getDeviceNumber() const {
	return deviceNumber_;
}

bool Device::isWrongChecksum() const {
	return isWrongChecksum_;
}

bool Device::isMessageLost() const {
	return isMessageLost_;
}
}
