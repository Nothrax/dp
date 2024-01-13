#include <gateway/endpoint/generator/GeneratorEndpoint.hpp>
#include <gateway/common_tools/MessageTools.hpp>



namespace gateway::endpoint::generator {

bool GeneratorEndpoint::initialize() {
	initialized_ = true;
	return initialized_;
}

std::shared_ptr<structures::DeviceMessage> GeneratorEndpoint::getMessage(unsigned int timeoutMs) {
	if(!initialized_) {
		throw std::runtime_error("Generator endpoint was not initialized");
	}
	auto message = std::make_shared<structures::DeviceMessage>();
	message->protocolVersion = 3;
	message->deviceType = 1;
	message->deviceNumber = 1;
	message->flags = messageCounter_;

	std::memcpy(&message->values[0], &temperature_, sizeof(temperature_));
	std::memcpy(&message->values[1], &humidity_, sizeof(humidity_));
	message->values[2] = co2_;

	messageCounter_++;
	if(messageCounter_ > MAX_MESSAGE_INDEX) {
		messageCounter_ = 0;

	}
	message->checkSum = common_tools::MessageTools::calculateChecksum((uint8_t *)message.get(),
																	  sizeof(structures::DeviceMessage) -
																	  sizeof(uint32_t));
	std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs/2));
	return message;
}
}