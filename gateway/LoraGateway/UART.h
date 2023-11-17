#pragma once

#include <string>
#include <wiringSerial.h>
#include "Logger/Logger.h"

class UART {
public:
    static UART * getInstance();
    void startUart(const std::string& device, uint32_t baudRate);
    void sendBuffer(const uint8_t *buffer, uint32_t bufferSize);
    void readBuffer(uint8_t *buffer, uint32_t bufferSize);
    ~UART();
private:
    static UART* instance;
    int fileDescriptor = -1;
};
