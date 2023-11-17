
#include "include/iot/LoRa.h"
#include "include/iot/logger/Logger.h"
#include "include/iot/Arguments.h"

//do dokumentace
//https://www.raspberrypi.org/documentation/configuration/uart.md
//https://www.electronicwings.com/raspberry-pi/raspberry-pi-uart-communication-using-python-and-c


//todo odchytavat signal a ukoncovat komunikaci
uint8_t calculateCheckSum(const uint8_t *data, size_t dataSize);

int main(int argc, char **argv) {
    //todo inicializace, nejspis config
	std::cout << "Starting LoRa gateway\n";
    SettingsStruct loRaSettings;

    Logger::initLogger("/var/log/", true);

    LoRa *lora = LoRa::getInstance();
    lora->startLoRa(loRaSettings);

    uint8_t messageCounter = 0;
    while(true){
        LoRaMessageStruct loRaMessage = lora->getMessage();



        //todo toto do nejakeho zpracovaniiii

        if(loRaMessage.flags & 0b10000000){
            Logger::logError("Wrong check sum on 433MHZ RF radio");
        }
        std::cout << (loRaMessage.flags & 0b01000000) << std::endl;
        if(loRaMessage.flags & 0b01000000){
            Logger::logError("Message got lost on 433MHZ RF radio");
        }
        if(messageCounter+1 != (loRaMessage.flags & 0b00001111)){
            //todo jinak
            if(messageCounter != 0){
                Logger::logError("Message got lost on Lora");
            }
            messageCounter = (loRaMessage.flags & 0b00001111) ;
        }else{
            messageCounter++;
            if(messageCounter >= 15){
                messageCounter = 0;
            }
        }

        uint32_t checkSum = calculateCheckSum((uint8_t *)&loRaMessage, sizeof(LoRaMessageStruct) - sizeof(uint32_t));
        if(checkSum != loRaMessage.checkSum){
            //todo opravit
/*            std::stringstream ss;
            for(int i=0; i<sizeof(LoRaMessageStruct); ++i)
                ss << std::hex << receiveBuffer[i];
            std::string messageString = ss.str();

            Logger::logError("Wrong check sum. Message is: " + ss.str()
                             + " checksum is: " + std::to_string(loRaMessage.checkSum) + " checksum should be: " + std::to_string(checkSum));*/
            Logger::logError("Wrong checksum");
        }

        float floatValues[2];
        memcpy((void *)floatValues, (void *) loRaMessage.values, 8);
        printf("version: %d, unit number: %d, flags: %02x, temp: %f hum: %f co2: %d sum: %d\n"
                ,loRaMessage.protocolVersion, loRaMessage.unitNumber, loRaMessage.flags, floatValues[0], floatValues[1], loRaMessage.values[2], loRaMessage.checkSum);
    }
}

uint8_t calculateCheckSum(const uint8_t *data, size_t dataSize) {
    uint8_t sum = 0;
    for(int index = 0; index < dataSize; index++){
        sum += data[index];
    }
    return sum;
}
