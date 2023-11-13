#include <SoftwareSerial.h>

#define TX_PIN 2
#define RX_PIN 3
#define M0_PIN 4
#define M1_PIN 5

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
    uint8_t setSettings[6] = {0xc0,0x11,0x22, 0x1a, 0x17, 0xa4};

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

    //   put your main code here, to run repeatedly:
    uint8_t sendBuffer[27] = {0x14,0x24,0x02,0x01};
    sendBuffer[26] = 0x42;
    sended = loraSerial.write(sendBuffer,27);
    if(sended == 27){
        Serial.println(0x01, HEX);
    }

    delay(3000);
}
char counter = 0;
void loop() {


}