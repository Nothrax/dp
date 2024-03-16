#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/common_tools/EnumTools.hpp>
#include <gateway/input_protocol/EndpointFactory.hpp>
#include <gateway/output_protocol/OutputFactory.hpp>
#include <gateway/device/DeviceFactory.hpp>



namespace gateway {

void Gateway::start() {
	logger::Logger::logInfo("Starting gateway of type LoRa");

	if(!initialize()) {
		logger::Logger::logError("Failed to initialize gateway");
		return;
	}

	while(!context_->context.stopped()) {
		auto message = endpoint_->getMessage(messageReceiveTimeoutMs_);
		if(message) {
			logger::Logger::logInfo("Received message from endpoint");
			processMessage(*message);
		}else{
			logger::Logger::logError("Failed to receive message from endpoint");
		}
	}
	logger::Logger::logInfo("Stopping gateway of type LoRa");
}

bool Gateway::initialize() {
	endpoint_ = input_protocol::EndpointFactory::createEndpoint(context_);
	output_ = output_protocol::OutputFactory::createOutput(context_);

	for(const auto &supportedDevice: context_->settings->getSupportedDevices()) {
		if(devices_.count(supportedDevice.deviceType) == 0) {
			devices_[supportedDevice.deviceType] = std::map<uint32_t, std::shared_ptr<device::Device>>();
		}

		if(devices_.at(supportedDevice.deviceType).count(supportedDevice.deviceNumber) == 0) {
			devices_.at(supportedDevice.deviceType)[supportedDevice.deviceNumber] = device::DeviceFactory::createDevice(
					supportedDevice.deviceType,
					supportedDevice.deviceNumber);
		}
	}

	if(!endpoint_->initialize()) {
		logger::Logger::logError("Failed to initialize endpoint of type {}",
								 common_tools::EnumTools::enumToString(context_->settings->getDeviceType()));
		return false;
	}
	if(!output_->initialize()) {
		logger::Logger::logError("Failed to initialize output of type {}, reconnect will be attempted",
								 common_tools::EnumTools::enumToString(context_->settings->getOutputType()));
	}
	return true;
}

void Gateway::processMessage(const input_protocol::InputProtocolMessage &message) {
	if(devices_.count(message.deviceType) == 0 || devices_.at(message.deviceType).count(message.deviceNumber) == 0) {
		logger::Logger::logError("Received message from unsupported device {} of type {}", message.deviceNumber,
								 message.deviceType);
		return;
	}

	auto device = devices_.at(message.deviceType).at(message.deviceNumber);
	auto parsedMessage = device->parseMessage(message);
	if(parsedMessage) {
		logger::Logger::logInfo("Parsed message from device {} of type {}", message.deviceNumber,
								common_tools::EnumTools::enumToString(device->getDeviceType()));
		output_->sendMessage(parsedMessage);
	} else {
		logger::Logger::logError("Failed to parse message from device {} of type {}", message.deviceNumber,
								 message.deviceType);
	}
}


}