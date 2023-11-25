#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/common_tools/MessageTools.hpp>
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
		auto message = endpoint->getMessage(messageReceiveTimeoutMs_);
		if(message){
			processMessage(*message);
		}
	}
	logger::Logger::logInfo("Stopping gateway of type LoRa");
}

bool Gateway::initialize() {
	endpoint = endpoint::EndpointFactory::createEndpoint(context_);
	csvWriter_ = std::make_unique<output::csv::CsvOutput>(context_->settings->getCsvPath(), context_->settings->getNumberOfCsvEntries());

	if(!endpoint->initialize()) {
		logger::Logger::logError("Failed to initialize endpoint of type " + common_tools::EnumTools::enumToString(context_->settings->getDeviceType()));
		return false;
	}
	return true;
}

void Gateway::processMessage(const structures::DeviceMessage &message) {
	///todo based on device type pass to specific device
	if(message.flags & 0b10000000) {
		logger::Logger::logError("Wrong check sum on 433MHZ RF radio");
	}
	if(message.flags & 0b01000000) {
		logger::Logger::logError("Message got lost on 433MHZ RF radio");
	}
	//todo message counter per device
	if(messageCounter_ + 1 != (message.flags & 0b00001111)) {
		//todo jinak
		if(messageCounter_ != 0) {
			logger::Logger::logError("Message got lost on Lora");
		}
		messageCounter_ = (message.flags & 0b00001111);
	} else {
		messageCounter_++;
		if(messageCounter_ >= 15) {
			messageCounter_ = 0;
		}
	}

	uint32_t checkSum = common_tools::MessageTools::calculateChecksum((uint8_t *)&message, sizeof(structures::DeviceMessage) - sizeof(uint32_t));
	if(checkSum != message.checkSum) {
		logger::Logger::logError("Wrong checksum");
	}

	logger::Logger::logInfo("Received message from LoRa endpoint");
	csvWriter_->write(message);

}


}