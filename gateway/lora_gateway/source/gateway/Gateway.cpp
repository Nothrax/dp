#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/common_tools/EnumTools.hpp>
#include <gateway/endpoint/EndpointFactory.hpp>
#include <gateway/output/OutputFactory.hpp>



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
			processMessage(*message);
		}
	}
	logger::Logger::logInfo("Stopping gateway of type LoRa");
}

bool Gateway::initialize() {
	endpoint_ = endpoint::EndpointFactory::createEndpoint(context_);
	output_ = output::OutputFactory::createOutput(context_);

	if(!endpoint_->initialize()) {
		logger::Logger::logError("Failed to initialize endpoint of type " +
								 common_tools::EnumTools::enumToString(context_->settings->getDeviceType()));
		return false;
	}
	if(!output_->initialize()) {
		logger::Logger::logError("Failed to initialize output of type " +
								 common_tools::EnumTools::enumToString(context_->settings->getOutputType()));
		return false;
	}
	return true;
}

void Gateway::processMessage(const structures::DeviceMessage &message) {
	if(devices_.count(message.deviceType) == 0) {
		devices_[message.deviceType] = std::map<uint32_t, std::shared_ptr<device::Device>>();
	}

	if(devices_.at(message.deviceType).count(message.deviceNumber) == 0) {
		devices_.at(message.deviceType)[message.deviceNumber] = std::make_shared<device::Device>(message.deviceType,
																								 message.deviceNumber);
	}

	auto device = devices_.at(message.deviceType).at(message.deviceNumber);
	if(device->parseMessage(message)) {
		logger::Logger::logInfo("Parsed message from device " + std::to_string(message.deviceNumber) + " of type " +
								 common_tools::EnumTools::enumToString(message.deviceType));
		output_->writeFromDevice(device);
	} else {
		logger::Logger::logError("Failed to parse message");
	}
}


}