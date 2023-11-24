#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>



namespace gateway {

void Gateway::start() {
	logger::Logger::logInfo("Starting gateway of type LoRa");

	if(!initialize()) {
		logger::Logger::logError("Failed to initialize gateway of type LoRa");
		return;
	}


	while(!context_->context.stopped()) {
		//todo timeout as class member
		auto message = loraEndpoint_->getMessage(1000);
		if(message){
			processMessage(*message);
		}
	}
	logger::Logger::logInfo("Stopping gateway of type LoRa");
}

//todo move to utils
uint8_t Gateway::calculateCheckSum(const uint8_t *data, size_t dataSize) {
	uint8_t sum = 0;
	for(int index = 0; index < dataSize; index++) {
		sum += data[index];
	}
	return sum;
}

bool Gateway::initialize() {
	loraEndpoint_ = std::make_unique<device::LoraEndpoint>(context_);
	csvWriter_ = std::make_unique<cloud::CsvWriter>(context_->settings->getCsvPath());

	if(!loraEndpoint_->initialize()) {
		logger::Logger::logError("Failed to initialize LoRa endpoint");
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

	uint32_t checkSum = calculateCheckSum((uint8_t *)&message, sizeof(structures::DeviceMessage) - sizeof(uint32_t));
	if(checkSum != message.checkSum) {
		logger::Logger::logError("Wrong checksum");
	}

	//todo create factory for devices that will output the data
/*		float floatValues[2];
		memcpy((void *)floatValues, (void *) loRaMessage.values, 8);
		printf("version: %d, unit number: %d, flags: %02x, temp: %f hum: %f co2: %d sum: %d\n"
				,loRaMessage.protocolVersion, loRaMessage.unitNumber, loRaMessage.flags, floatValues[0], floatValues[1], loRaMessage.values[2], loRaMessage.checkSum);*/
	logger::Logger::logInfo("Received message from LoRa endpoint");
	csvWriter_->write(message);

}


}