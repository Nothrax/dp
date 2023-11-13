//
// Created by root on 24.08.20.
//

#ifndef LORAGATEWAY_STRUCTURES_H
#define LORAGATEWAY_STRUCTURES_H

#define M0_PIN  4  // aka BCM pin 23
#define M1_PIN  5  // aka BCM pin 24
#define LOW 0
#define HIGH 1

//todo structura pro nastaveni lory


#pragma pack(push, 1)
//todo ze zpravy udelat tridu?
struct LoRaMessageStruct{
    uint8_t protocolVersion = 0;
    uint16_t unitNumber = 0;
    uint8_t flags = 0;
    uint32_t values[5] = {0};
    uint8_t checkSum = 0;
};
#pragma pack(pop)

struct SettingsStruct{
    std::string uartDevice = "/dev/ttyS0";
    uint32_t baudRate = 9600;
};

#endif //LORAGATEWAY_STRUCTURES_H




