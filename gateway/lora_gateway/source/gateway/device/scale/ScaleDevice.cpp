#include <gateway/device/scale/ScaleDevice.hpp>
#include <gateway/device/scale/ScaleEntry.hpp>
#include <gateway/common_tools/TimeTools.hpp>

#include <cstring>



namespace gateway::device::scale {
bool ScaleDevice::parseDeviceSpecificFlags(const uint8_t &flags) {
	return true;
}

std::shared_ptr<Entry> ScaleDevice::parseMessage(const input_protocol::InputProtocolMessage &message) {
	auto scaleMessage = std::make_shared<ScaleEntry>(message.deviceNumber);
	isCorrectChecksum_ = true;
	isMessageLost_ = false;

	if(!parseCommonHeader(message)) {
		return nullptr;
	}
	if(!parseDeviceSpecificFlags(message.flags)) {
		return nullptr;
	}

	scaleMessage->setCurrentTimestampMs(common_tools::TimeTools::getUnixTimestampMs());
	scaleMessage->setFlags(message.flags);

	float temperature, humidity, weight;
	std::memcpy((void *)&temperature, (void *)&message.values[0], sizeof(float));
	std::memcpy((void *)&humidity, (void *)&message.values[1], sizeof(float));
	std::memcpy((void *)&weight, (void *)&message.values[2], sizeof(float));
	scaleMessage->setTemperature(temperature);
	scaleMessage->setHumidity(humidity);
	scaleMessage->setWeight(weight);

	return scaleMessage;
}

}