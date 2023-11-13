//
// Created by root on 24.08.20.
//

#ifndef LORAGATEWAY_UART_H
#define LORAGATEWAY_UART_H

#include <string>
#include <wiringSerial.h>
#include "Logger/Logger.h"

class UART {
public:
    static UART * getInstance();
    void startUart(std::string device, uint32_t baudRate);
    void sendBuffer(uint8_t *buffer, uint32_t bufferSize);
    void readBuffer(uint8_t *buffer, uint32_t bufferSize);
    ~UART();
private:
    static UART* instance;
    int fileDescriptor = -1;
};


#endif //LORAGATEWAY_UART_H
