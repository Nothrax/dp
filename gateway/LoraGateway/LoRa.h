//
// Created by root on 24.08.20.
//
#include <cstdio>
#include <unistd.h>
#include <wiringPi.h>
#include <sstream>

#include "Logger/Logger.h"
#include "Structures.h"
#include "UART.h"

#ifndef LORAGATEWAY_LORA_H
#define LORAGATEWAY_LORA_H

//todo udelat jako singleton pro jednoduche smazani
class LoRa {
public:
    static LoRa * getInstance();
    void startLoRa(SettingsStruct settings);
    ~LoRa();
    LoRaMessageStruct getMessage();
private:
    enum modeSelection { normalMode, wakeUpMode, powerSavingMode, settingMode};
    static LoRa * instance;
    UART *uart = nullptr;
    SettingsStruct loraSettings;
    //todo naplnit z konstruktoru
    uint8_t settingsRegister[6] = { 0xc0, 0x14, 0x24, 0x1a, 0x02, 0xa4};

    //todo nastaveni jednotlivych parametru
    void initializeUart();
    void initializeModePins();
    void setMode(modeSelection mode);
    void initializeLora();
};


#endif //LORAGATEWAY_LORA_H
