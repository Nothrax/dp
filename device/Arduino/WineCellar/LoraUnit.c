#include <SoftwareSerial.h>
#include <VirtualWire.h>

#define TX_PIN 2
#define RX_PIN 3
#define M0_PIN 4
#define M1_PIN 5
#define WIRELESS_PIN 7

#define GATEWAY_ADDRESS_SIZE 3

#pragma pack(push, 1)

struct RFMessage {
    uint32_t counter;
    float temperature;
    float humidity;
    uint32_t carbonDioxide;
    uint32_t checkSum;
};

struct LoRaMessage{
    uint8_t protocolVersion = 1;
    uint16_t unitNumber = 3;
    uint8_t flags = 0;
    uint32_t values[5] = {0};
    uint8_t checkSum = 0;
};
#pragma pack(pop)
uint8_t checkSum8b(uint8_t *data, size_t dataSize);
uint32_t checkSum32b(uint8_t *data, size_t dataSize);

SoftwareSerial loraSerial(TX_PIN, RX_PIN);

void setup() {
    Serial.begin(9600);
    //zacatek komunikace s uartem lory
    loraSerial.begin(9600);

    //nastaveni m0 a m1 do nastavovaciho rezimu
    pinMode(M0_PIN, OUTPUT);
    pinMode(M1_PIN, OUTPUT);
    digitalWrite(M0_PIN, HIGH);
    digitalWrite(M1_PIN, HIGH);


    delay(500);

    //pole s nastavenim registru, viz dokumentace
    uint8_t setSettings[6] = {0xc0, 0x11, 0x22, 0x1a, 0x17, 0xa4};

    int sended = loraSerial.write(setSettings, 6);
    if(sended == 6){
        Serial.print("poslal jsem 6\n");
    }

    delay(100);

    while(loraSerial.available() > 0){
        int readed = loraSerial.read();
        Serial.print(readed, HEX);
    }

    Serial.print("\n");
    Serial.println("Setting transmition mode");
    digitalWrite(M0_PIN, LOW);
    digitalWrite(M1_PIN, LOW);
    delay(500);


    //433MHZ RF komunikace
    // nastavení typu bezdrátové komunikace
    vw_set_ptt_inverted(true);
    // nastavení rychlosti přenosu v bitech za sekundu
    vw_setup(512);
    // nastavení čísla datového pinu pro přijímač
    vw_set_rx_pin(WIRELESS_PIN);
    // nastartování komunikace po nastaveném pinu
    vw_rx_start();


}
uint32_t rfCounter = 0;
uint8_t loraCounter = 0;

void loop() {
    RFMessage rfMessage;
    uint8_t rfMessageSize = sizeof(RFMessage);

    LoRaMessage loraMessage;
    loraMessage.protocolVersion = 1;
    loraMessage.unitNumber = 3;
    // 1b wrong cheksum received, 1b lost message, 2b reserved, 4b counter
    loraMessage.flags = 0;
    //adding message counter to flags
    loraMessage.flags = loraCounter & 0b00001111;
    uint8_t gatewayAddress[3] = {0x14,0x24,0x02};

    //todo lora message counter

    if (vw_get_message((uint8_t *)&rfMessage, &rfMessageSize)) {
        uint32_t calculatedCheckSum = checkSum32b((uint8_t *) &rfMessage, rfMessageSize-sizeof(uint32_t));
        if(calculatedCheckSum != rfMessage.checkSum){
            Serial.println("Wrong Check Sum");

            loraMessage.flags |= 0b1000000;
        }else{
            memcpy((void *) loraMessage.values, (void *)&rfMessage.temperature, 12);
            loraMessage.values[3] = 0;
            loraMessage.values[4] = 0;
        }
        //check if message got lost
        rfCounter++;
        if(rfCounter != rfMessage.counter){
            //first message of program
            if(rfCounter != 0){
                loraMessage.flags |= 0b01000000;
            }
            rfCounter = rfMessage.counter;
        }

        loraMessage.checkSum = checkSum8b((uint8_t *)&loraMessage, sizeof(struct LoRaMessage) - sizeof(uint32_t));
        int sended = loraSerial.write(gatewayAddress,GATEWAY_ADDRESS_SIZE);
        sended += loraSerial.write((uint8_t *)&loraMessage,sizeof(LoRaMessage));
        if(sended == sizeof(LoRaMessage)+ GATEWAY_ADDRESS_SIZE){
            Serial.println("odeslano");
        }
        //counting lora messages on 4 bits, 0-15
        loraCounter++;
        if(loraCounter > 15){
            loraCounter = 0;
        }

    }
}

uint32_t checkSum32b(uint8_t *data, size_t dataSize){
    uint32_t sum = 0;
    for(int index = 0; index < dataSize; index++){
        sum += data[index];
    }
    return sum;
}

uint8_t checkSum8b(uint8_t *data, size_t dataSize){
    uint8_t sum = 0;
    for(int index = 0; index < dataSize; index++){
        sum += data[index];
    }
    return sum;
}