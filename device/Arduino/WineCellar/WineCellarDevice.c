#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VirtualWire.h>
#include "MHZ19.h"
#include "DHT.h"
//https://navody.arduino-shop.cz/navody-k-produktum/detektor-oxidu-uhliciteho-mh-z19b.html

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);
struct Message{
    uint32_t counter;
    float temperature;
    float humidity;
    uint32_t carbonDioxide;
    uint32_t checkSum;
};

//vlhkost a teplota DHT22
#define DHTPIN 2                                 //číslo pinu Arduina, kam je připojený DATA pin senzoru DHT
#define DHTTYPE DHT22                            //senzor je typp DHT22
DHT dht(DHTPIN, DHTTYPE);                        // do proměné dht uložíme údaje o již nadefinovaném senzoru

// CO2 a teplota mhz19
#define MHZ19_PWN_PIN 9
#define MHZ19_RX_PIN 11                          //nastavení propojovacích pinů CO2 senzor !pozor tx s rx a rx s tx
#define MHZ19_TX_PIN 10
MHZ19 *mhz19_uart = new MHZ19(MHZ19_RX_PIN, MHZ19_TX_PIN);   // vytvoření objektů z knihovny CO2 senzor mhz19
MHZ19 *mhz19_pwm = new MHZ19(MHZ19_PWN_PIN);

//433MHz komunikace
#define WIRELESS_PIN 7
#define LED_PIN 8
int messageCounter = 0;

void sendData(uint8_t *data, int dataSize);
uint32_t calculateCheckSum(uint8_t *data, int dataSize);

void setup() {
    lcd.init();                                   //nastaveni lcd
    lcd.backlight();
    lcd.setCursor(0,0);

    //todo led led led
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(9600);                           // zahájení komunikace po sériové lince

    mhz19_uart->begin(MHZ19_RX_PIN, MHZ19_TX_PIN);// zahájení komunikace se senzorem mhz19 přes UART
    mhz19_uart->setAutoCalibration(false);        // vypnutí autokalibrace

    dht.begin();                                  // aktivujeme komunikaci senzoru DHT

    // nastavení typu bezdrátové komunikace
    vw_set_ptt_inverted(true);
    // nastavení rychlosti přenosu v bitech za sekundu
    vw_setup(512);
    // nastavení čísla datového pinu pro vysílač
    vw_set_tx_pin(WIRELESS_PIN);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW); 
    delay(1000);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
}

void loop() {
    //MHZ19
    measurement_t m = mhz19_uart->getMeasurement();
    int co2 = (mhz19_pwm->getPpmPwm() + map(m.co2_ppm, 0, 5000, 0, 2000))/2;

    //DHT22
    //todo isnan(hum) || isnan(temp)
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    if(co2 > 4900){  
      digitalWrite(LED_PIN, HIGH); 
    }else{
      digitalWrite(LED_PIN, LOW); 
    }

    //zapis na lcd
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.print(co2);
    lcd.print("PPM ");
    //lcd.print(m.temperature); //teplota z MHZ19
    lcd.print(temp);
    lcd.print("\xDF" "C");

    lcd.setCursor(0,1);
    lcd.print(hum);
    lcd.print("%");

    //todo cervena ledka

    Message message = {messageCounter,temp,hum,co2, 0};
    messageCounter++;
    message.checkSum = calculateCheckSum((uint8_t *)&message, 20);
    sendData((uint8_t *)&message, sizeof(Message));
    delay(15000);
}

void sendData(uint8_t *data, int dataSize){
    vw_send((uint8_t *)data, dataSize);
    vw_wait_tx();
}

uint32_t calculateCheckSum(uint8_t *data, int dataSize){
    uint32_t checkSum = 0;
    for(int i = 0; i < dataSize; i++){
        checkSum += data[i];
    }
    return checkSum;
}
