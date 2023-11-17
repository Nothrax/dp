#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VirtualWire.h>
#include "MHZ19.h"
#include "DHT.h"

/// Humidity and temperature sensor DHT22 defines
/// Pin number of arduino where the DATA pin of DHT sensor
#define DHT_PIN 2
// Sensor is of type DHT22
#define DHT_TYPE DHT22
/// Instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

/// CO2 and temperature sensor MHZ19 defines
/// PWM pin of MHZ19
#define MHZ19_PWN_PIN 9
/// RX pin of MHZ19
#define MHZ19_RX_PIN 11
/// TX pin of MHZ19
#define MHZ19_TX_PIN 10
/// 433MHz communication chip defines

/// RF board (NiceRF 433MHz wl101-341) specific settings
#define RF_WIRELESS_PIN 7
#define RF_BITS_PER_SECOND 512

/// Pin of indication led
#define LED_PIN 8
/// Data rate on serial port
#define SERIAL_DATA_RATE 9600

/// CO2 threshold, after reaching this value, red led will be turned on
#define CO2_THRESHOLD 4900;
/// How often will the values be measured and sended using RF board
#define MEASUREMENT_DELAY_MS 15000

/// Instances of MHZ19 sensor
MHZ19 *mhz19_uart = new MHZ19(MHZ19_RX_PIN, MHZ19_TX_PIN);
MHZ19 *mhz19_pwm = new MHZ19(MHZ19_PWN_PIN);

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

/**
 * Definition of the RF message
**/
struct RFMessage {
	/// Message counter
	uint32_t counter;
	/// Current temperature
	float temperature;
	/// Current humidity
	float humidity;
	/// Currenc CO2 level
	uint32_t carbonDioxide;
	/// Check sum
	uint32_t checkSum;
};

/// Message counter of RF messages
int rfMessageCounter { 0 };

/**
 * Send data using 433Mhz chip
**/
void sendData(const uint8_t *data, int dataSize);

/**
 * Calculate 32bit checksum from given values
**/
uint32_t calculateCheckSum(const uint8_t *data, int dataSize);

void setup() {
	/// LCD screen init
	lcd.init();
	lcd.backlight();
	lcd.setCursor(0, 0);

	/// Set led pin as output
	pinMode(LED_PIN, OUTPUT);

	// Serial communication
	Serial.begin(SERIAL_DATA_RATE);

	/// Initialization of communication with MHZ19
	mhz19_uart->begin(MHZ19_RX_PIN, MHZ19_TX_PIN);
	mhz19_uart->setAutoCalibration(false);


	/// Initialization of communication with DHT
	dht.begin();

	/// 433Mhz chip setup
	vw_set_ptt_inverted(true);
	vw_setup(RF_BITS_PER_SECOND);
	vw_set_tx_pin(WIRELESS_PIN);

	/// Init done indication using leds
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	delay(1000);
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
}

void loop() {
	//MHZ19 reading
	measurement_t m = mhz19_uart->getMeasurement();
	int co2 = (mhz19_pwm->getPpmPwm() + map(m.co2_ppm, 0, 5000, 0, 2000))/2;

	/// DHT22 reading
	//todo isnan(hum) || isnan(temp)
	float hum = dht.readHumidity();
	float temp = dht.readTemperature();

	/// If CO2 is over threshold, turn on warning led
	if(co2 > co2Threshold) {
		digitalWrite(LED_PIN, HIGH);
	} else {
		digitalWrite(LED_PIN, LOW);
	}

	/// Lcd write of measured values
	lcd.setCursor(0, 0);
	lcd.clear();
	lcd.print(co2);
	lcd.print("PPM ");
	lcd.print(temp);
	lcd.print("\xDF" "C");

	lcd.setCursor(0, 1);
	lcd.print(hum);
	lcd.print("%");


	Message message = { messageCounter, temp, hum, co2, 0 };
	messageCounter++;
	message.checkSum = calculateCheckSum((uint8_t*) & message, 20);
	sendData((uint8_t*) & message, sizeof(Message));
	delay(MEASUREMENT_DELAY_MS);
}

void sendData(const uint8_t *data, int dataSize) {
	vw_send((uint8_t *)data, dataSize);
	vw_wait_tx();
}

uint32_t calculateCheckSum(const uint8_t *data, int dataSize) {
	uint32_t checkSum = 0;
	for(int i = 0; i < dataSize; i++) {
		checkSum += data[i];
	}
	return checkSum;
}
