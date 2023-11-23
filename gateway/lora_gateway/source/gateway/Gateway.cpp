#include <gateway/Gateway.hpp>
#include <gateway/logger/Logger.hpp>

namespace gateway{

void Gateway::start() {
	logger::Logger::logInfo("Starting gateway of type LoRa");
	loraEndpoint_ = std::make_unique<device::LoraEndpoint>(context_);
	csvWriter_ = std::make_unique<cloud::CsvWriter>("/home/nothrax/data");

	if(!loraEndpoint_->initialize()){
		logger::Logger::logError("Failed to initialize LoRa endpoint");
		return;
	}
	uint8_t messageCounter = 0;
	//todo boost context
	while(!context_->context.stopped()){
		auto loRaMessage = loraEndpoint_->getMessage();



		//todo toto do nejakeho zpracovaniiii

		if(loRaMessage.flags & 0b10000000){
			logger::Logger::logError("Wrong check sum on 433MHZ RF radio");
		}
		if(loRaMessage.flags & 0b01000000){
			logger::Logger::logError("Message got lost on 433MHZ RF radio");
		}
		if(messageCounter+1 != (loRaMessage.flags & 0b00001111)){
			//todo jinak
			if(messageCounter != 0){
				logger::Logger::logError("Message got lost on Lora");
			}
			messageCounter = (loRaMessage.flags & 0b00001111) ;
		}else{
			messageCounter++;
			if(messageCounter >= 15){
				messageCounter = 0;
			}
		}

		uint32_t checkSum = calculateCheckSum((uint8_t *)&loRaMessage, sizeof(structures::LoraMessage) - sizeof(uint32_t));
		if(checkSum != loRaMessage.checkSum){
			logger::Logger::logError("Wrong checksum");
		}

		//todo create factory for devices that will output the data
/*		float floatValues[2];
		memcpy((void *)floatValues, (void *) loRaMessage.values, 8);
		printf("version: %d, unit number: %d, flags: %02x, temp: %f hum: %f co2: %d sum: %d\n"
				,loRaMessage.protocolVersion, loRaMessage.unitNumber, loRaMessage.flags, floatValues[0], floatValues[1], loRaMessage.values[2], loRaMessage.checkSum);*/
		logger::Logger::logInfo("Received message from LoRa endpoint");
		csvWriter_->write(loRaMessage);
	}
	logger::Logger::logInfo("Stopping gateway of type LoRa");
}

uint8_t Gateway::calculateCheckSum(const uint8_t *data, size_t dataSize) {
	uint8_t sum = 0;
	for(int index = 0; index < dataSize; index++){
		sum += data[index];
	}
	return sum;
}


}