//
// Created by root on 24.08.20.
//

#include "LoRa.h"

//todo
//Konstruktor s parametry pro loru


void LoRa::startLoRa(SettingsStruct settings) {
    loraSettings = settings;
    initializeUart();
    initializeModePins();
    initializeLora();
}

LoRaMessageStruct LoRa::getMessage() {
    LoRaMessageStruct loRaMessage;
    uint8_t receiveBuffer[sizeof(LoRaMessageStruct)];

    uart->readBuffer(receiveBuffer, sizeof(LoRaMessageStruct));
    memcpy((void *) &loRaMessage, (void *)receiveBuffer, sizeof(LoRaMessageStruct));

    return loRaMessage;
}

LoRa::~LoRa() {
    delete uart;
}

LoRa* LoRa::instance = nullptr;

LoRa * LoRa::getInstance() {
    if(instance == nullptr){
        instance = new LoRa();
    }
    return instance;
}

void LoRa::initializeModePins() {
    if (wiringPiSetup () == -1){
        Logger::logError( "Wiring pi setup failed\n");
        exit(EXIT_FAILURE);
    }
    pinMode (M0_PIN, OUTPUT);
    pinMode (M1_PIN, OUTPUT);
}

void LoRa::initializeUart() {
    uart = UART::getInstance();
    uart->startUart(loraSettings.uartDevice, loraSettings.baudRate);
}

void LoRa::setMode(modeSelection mode) {
    //check datasheet for E32-868T20D for more info
    switch(mode){
        case normalMode:
            digitalWrite(M0_PIN, LOW);
            digitalWrite(M1_PIN, LOW);
            break;
        case wakeUpMode:
            digitalWrite(M0_PIN, HIGH);
            digitalWrite(M1_PIN, LOW);
            break;
        case powerSavingMode:
            digitalWrite(M0_PIN, LOW);
            digitalWrite(M1_PIN, HIGH);
            break;
        case settingMode:
            digitalWrite(M0_PIN, HIGH);
            digitalWrite(M1_PIN, HIGH);
            break;
        default:
            Logger::logWarning("Trying to set non-existing mode, continuing without changing mode.");
            break;
    }
}

void LoRa::initializeLora() {
    uint8_t receiveRegister[6];
    //change lora mode to sleep mode that allows sending setting command
    setMode(settingMode);
    usleep(100);
    //send register with all parameters
    uart->sendBuffer(settingsRegister, 6);
    uart->readBuffer(receiveRegister, 6);

    usleep(100);
    //set mode back to wireless communication
    setMode(normalMode);
}