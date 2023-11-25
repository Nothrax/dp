#include <gateway/device/Device.hpp>
#include <gateway/common_tools/TimeTools.hpp>
#include <gateway/common_tools/MessageTools.hpp>
#include <gateway/logger/Logger.hpp>

#include <cstring>



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

	message_ = message;
	isWrongChecksum_ = false;
	isMessageLost_ = false;

	if(message.flags & wrongChecksumMask_) {
		logger::Logger::logError("Wrong check sum on 433MHZ RF radio");
		isWrongChecksum_ = true;
	}

	if(message.flags & messageLostMask_) {
		logger::Logger::logError("Message got lost on 433MHZ RF radio");
		isMessageLost_ = true;
	}

	uint8_t stamp = (message.flags & stampMask_);

	if(lastStamp_ != -1) {
		if(lastStamp_ + 1 != stamp) {
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

	return !isWrongChecksum_;
}

std::string Device::getCsvHeader() const {
	switch(deviceType_) {
		case structures::EDeviceType::E_WINE_CELLAR:
			return "timestamp_ms,protocol_version,device_type,device_number,flags,temperature,humidity,co2,check_sum\n";
		case structures::EDeviceType::E_BEE_SCALE:
			return "not implemented";
		default:
			return "";
	}
}

std::string Device::getCsvEntry() const {
	std::string entry;
	if(message_.deviceType == 1) {
		entry += std::to_string(common_tools::TimeTools::getUnixTimestampMs());
		entry += "," + std::to_string(message_.protocolVersion);
		entry += "," + std::to_string(message_.deviceType);
		entry += "," + std::to_string(message_.deviceNumber);
		entry += "," + std::to_string(message_.flags);

		float floatValues[2];
		std::memcpy((void *)floatValues, (void *)message_.values, 8);

		entry += "," + std::to_string(floatValues[0]);
		entry += "," + std::to_string(floatValues[1]);
		entry += "," + std::to_string(message_.values[2]);
		entry += "," + std::to_string(message_.checkSum);
		entry += "\n";
	} else if(message_.deviceType == 2) {
		return "not implemented";
	}

	return entry;
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
