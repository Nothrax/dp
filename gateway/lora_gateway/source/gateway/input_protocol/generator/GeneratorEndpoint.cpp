#include <gateway/input_protocol/generator/GeneratorEndpoint.hpp>
#include <gateway/common_tools/MessageTools.hpp>
#include <gateway/common_tools/EnumTools.hpp>



namespace gateway::input_protocol::generator {

bool GeneratorEndpoint::initialize() {
	initialized_ = true;
	return initialized_;
}

std::shared_ptr<input_protocol::InputProtocolMessage> GeneratorEndpoint::getMessage(unsigned int timeoutMs) {
	if(!initialized_) {
		throw std::runtime_error("Generator endpoint was not initialized");
	}
	auto message = std::make_shared<input_protocol::InputProtocolMessage>();
	message->protocolVersion = 3;
	message->deviceType = context_->settings->getGeneratorDeviceType();
	message->deviceNumber = context_->settings->getGeneratorDeviceNumber();
	message->flags = messageCounter_;

	auto deviceType = common_tools::EnumTools::valueToEnum<input_protocol::EDeviceType>((uint32_t)message->deviceType);
	if(deviceType == input_protocol::EDeviceType::E_WINE_CELLAR) {
		std::memcpy(&message->values[0], &temperatureCellar_, sizeof(temperatureCellar_));
		std::memcpy(&message->values[1], &humidityCellar_, sizeof(humidityCellar_));
		message->values[2] = co2Cellar_;
	} else if(deviceType == input_protocol::EDeviceType::E_BEE_SCALE) {
		std::memcpy(&message->values[0], &temperatureBee_, sizeof(temperatureBee_));
		std::memcpy(&message->values[1], &humidityBee_, sizeof(humidityBee_));
		std::memcpy(&message->values[2], &weightBee_, sizeof(weightBee_));
	} else {
		throw std::runtime_error("Unsupported device type");
	}


	messageCounter_++;
	if(messageCounter_ > MAX_MESSAGE_INDEX) {
		messageCounter_ = 0;

	}
	message->checkSum = common_tools::MessageTools::calculateChecksum((uint8_t *)message.get(),
																	  sizeof(input_protocol::InputProtocolMessage) -
																	  sizeof(uint32_t));
	std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs/2));
	return message;
}
}