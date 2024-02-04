#include <gateway/device/cellar/CellarDevice.hpp>
#include <gateway/device/cellar/CellarMessage.hpp>
#include <gateway/common_tools/TimeTools.hpp>
#include <gateway/logger/Logger.hpp>

#include <cstring>



namespace gateway::device::cellar {


bool CellarDevice::parseDeviceSpecificFlags(const uint8_t &flags) {
	if(flags & MESSAGE_LOST_MASK) {
		logger::Logger::logError("Message got lost on 433MHZ RF radio");
		isMessageLost_ = true;
	}
	if(flags & WRONG_CHECKSUM_MASK) {
		logger::Logger::logError("Wrong check sum on 433MHZ RF radio");
		isCorrectChecksum_ = false;
	}
	return isCorrectChecksum_;
}

std::shared_ptr<Message> CellarDevice::parseMessage(const input_protocol::InputProtocolMessage &message) {
	auto cellarMessage = std::make_shared<CellarMessage>(message.deviceNumber);
	isCorrectChecksum_ = true;
	isMessageLost_ = false;

	if(!parseCommonHeader(message)) {
		return nullptr;
	}
	if(!parseDeviceSpecificFlags(message.flags)) {
		return nullptr;
	}

	cellarMessage->setCurrentTimestampMs(common_tools::TimeTools::getUnixTimestampMs());
	cellarMessage->setFlags(message.flags);

	float temperature, humidity;
	std::memcpy((void *)&temperature, (void *)&message.values[0], sizeof(float));
	std::memcpy((void *)&humidity, (void *)&message.values[1], sizeof(float));
	cellarMessage->setTemperature(temperature);
	cellarMessage->setHumidity(humidity);
	cellarMessage->setCo2(message.values[2]);

	return cellarMessage;
}
}