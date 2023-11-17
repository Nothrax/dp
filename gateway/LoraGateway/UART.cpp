#include "UART.h"
UART* UART::instance = nullptr;

UART* UART::getInstance() {
    if(instance == nullptr){
        instance = new UART();
    }
    return instance;
}

void UART::readBuffer(uint8_t *buffer, uint32_t bufferSize) {
    while(int dataAvailable = serialDataAvail(fileDescriptor) < bufferSize){
        if(dataAvailable < 0){
            Logger::logError("Failed reading ttyS0");
            exit(EXIT_FAILURE);
        }
    }
    for(int i = 0; i < bufferSize; i++){
        buffer[i] = serialGetchar(fileDescriptor);
    }
}

void UART::sendBuffer(const uint8_t *buffer, uint32_t bufferSize) {
    for(int i = 0; i < bufferSize; i++){
        serialPutchar(fileDescriptor,buffer[i]);
    }
}

UART::~UART() {
    serialClose(fileDescriptor);
    instance = nullptr;
}

void UART::startUart(const std::string& device, uint32_t baudRate) {
    fileDescriptor = serialOpen(device.c_str(), baudRate);
    if(fileDescriptor < 0){
        Logger::logError( "Cannot open device ttyS0\n");
        exit(EXIT_FAILURE);
    }
}