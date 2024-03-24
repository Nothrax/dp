#include <SoftwareSerial.h>
#include <VirtualWire.h>
#include <HX711.h>

/// LoRa board (E32-868T20D) specific settings
/// LoRa Tx pin number
#define LORA_TX_PIN 6
/// LoRa Rx pin number
#define LORA_RX_PIN 7
/// M0 and M1 pins decide mode of the board, see documentation for available modes
#define LORA_M0_PIN 8
#define LORA_M1_PIN 9

/// Data rate on serial port
#define SERIAL_DATA_RATE 9600

#define CALIBRATION_FACTOR -430
#define LOADCELL_DOUT_PIN1  2
#define LOADCELL_SCK_PIN1  3
#define LOADCELL_DOUT_PIN2  4
#define LOADCELL_SCK_PIN2  5


#pragma pack(push, 1)

/**
  * Definition of LoRa message
  * flags:
  * bit 7 - wrong cheksum received
  * bit 6 - lost message
  * bits 5-4 reserved
  * bits 3-0 message counter
  * | 7 | 6 | 5 4 | 3 2 1 0 |
**/
struct LoRaMessage {
	/// Version number of protocl
	uint8_t protocolVersion;
  /// Unit type
  uint8_t unitType;
	/// Unit identification number
	uint8_t unitNumber;
	/// Flags
	uint8_t flags { 0 };
	/// Measured values
	uint32_t values[5] { 0 };
	/// Checksum
	uint8_t checkSum { 0 };
};
#pragma pack(pop)

struct Measurement {
  float scale_1;
  float scale_2;
  float temperature;
  float humidity;
};


/// Can be edited start
/// LoRa E32-868T20D settings, see documentation
/// 0xc0 - settings command
/// 0x1424 - address of the device
/// 0x1a - 8N1 UART mode, 9600 UART baud rate, 2.4k Air Data Rate
/// 0x02 - channel of the device
/// 0x24 - Transparent transition mode, IO drive mode 0, Wireles wake-up time 1250ms, FEC off, transmission power 30dBm
uint8_t loraSettings[] = { 0xc0, 0x14, 0x24, 0x1a, 0x02, 0x24 };

/// LoRa Gateway protocol values
#define PROTOCOL_VERSION 3;
#define UNIT_NUMBER 1;
#define UNIT_TYPE 2;


HX711 scale1;
HX711 scale2;

/// LoRa message counter
uint8_t loraCounter { 0 };

/**
 * Send LoRa message
**/
void sendLoRaMessage(const Measurement& measurement);

/**
 * Make a measurement
**/
Measurement makeMeasurement();

/**
* Calculate 8 bit checksum of given data
**/
uint8_t checkSum8b(const uint8_t *data, size_t dataSize);


/// Lora board instance
SoftwareSerial loraSerial(LORA_TX_PIN, LORA_RX_PIN);

void setup() {
	Serial.begin(SERIAL_DATA_RATE);

	/// Start LoRa board communication on serial
	loraSerial.begin(SERIAL_DATA_RATE);

	/// Set LoRa board M0 and M1 pins to HIGH - sleep mode when settings can be uploaded
	pinMode(LORA_M0_PIN, OUTPUT);
	pinMode(LORA_M1_PIN, OUTPUT);
	digitalWrite(LORA_M0_PIN, HIGH);
	digitalWrite(LORA_M1_PIN, HIGH);

	/// Delay for pin settings to become stable
	delay(500);

	/// Upload settings to LoRa board
	int sended = loraSerial.write(loraSettings, sizeof(loraSettings));
	if(sended == sizeof(loraSettings)) {
		Serial.println("LoRa settings uploaded to the board");
	}

	delay(100);

	/// Wait for ACK from LoRa board
	while(loraSerial.available() > 0) {
		int readed = loraSerial.read();
		Serial.print(readed, HEX);
	}

	Serial.print("\n");
	Serial.println("Setting LoRa board into transmition mode");
	/// Set LoRa board M0 and M1 pins to LOW - UART and wireless channel are open, transparent transmission is on
	digitalWrite(LORA_M0_PIN, LOW);
	digitalWrite(LORA_M1_PIN, LOW);
	delay(500);

  scale1.begin(LOADCELL_DOUT_PIN1, LOADCELL_SCK_PIN1);
  scale2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2);
  scale1.set_scale(CALIBRATION_FACTOR); 
  scale2.set_scale(CALIBRATION_FACTOR);
  scale1.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale2.tare(); 

}

void loop() {
  Measurement measurement = makeMeasurement();
  sendLoRaMessage(measurement);
  delay(30000);
}

Measurement makeMeasurement(){
  Measurement measurement;
  measurement.scale_1 = 15.0;
  measurement.scale_2 = 20.0;
  measurement.humidity = 86.5;
  measurement.temperature = 22.3;
  return measurement;
}

void sendLoRaMessage(const Measurement& measurement) {
	LoRaMessage loraMessage;
	loraMessage.protocolVersion = PROTOCOL_VERSION;
	loraMessage.unitNumber = UNIT_NUMBER;
  loraMessage.unitType = UNIT_TYPE;
	loraMessage.flags = 0;
	/// Adding message counter to flags
	loraMessage.flags = loraCounter & 0b00001111;

  memcpy((void *)&loraMessage.values[0], (void *)&measurement.temperature, 4);
  memcpy((void *)&loraMessage.values[1], (void *)&measurement.humidity, 4);
  float weight = measurement.scale_1 + measurement.scale_2;
  memcpy((void *)&loraMessage.values[2], (void *)&weight, 4);
  loraMessage.values[3] = 0;
  loraMessage.values[4] = 0;


	loraMessage.checkSum = checkSum8b((uint8_t*) & loraMessage, sizeof(LoRaMessage) - sizeof(uint32_t));
	/// Send the LoRa message
	int sended = loraSerial.write((uint8_t*) & loraMessage, sizeof(LoRaMessage));
	if(sended == sizeof(LoRaMessage)) {
		Serial.println("LoRa Message sended");
	}

	/// Counting lora messages on 4 bits, 0-15
	loraCounter++;
	if(loraCounter > 15) {
		loraCounter = 0;
	}
}

uint8_t checkSum8b(const uint8_t *data, size_t dataSize) {
	uint8_t sum = 0;
	for(int index = 0; index < dataSize; index++) {
		sum += data[index];
	}
	return sum;
}