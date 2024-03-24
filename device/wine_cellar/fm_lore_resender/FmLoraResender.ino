#include <SoftwareSerial.h>
#include <VirtualWire.h>

/// LoRa board (E32-868T20D) specific settings
/// LoRa Tx pin number
#define LORA_TX_PIN 2
/// LoRa Rx pin number
#define LORA_RX_PIN 3
/// M0 and M1 pins decide mode of the board, see documentation for available modes
#define LORA_M0_PIN 4
#define LORA_M1_PIN 5

/// RF board (NiceRF 433MHz wl101-341) specific settings
#define RF_WIRELESS_PIN 7
#define RF_BITS_PER_SECOND 512

/// Data rate on serial port
#define SERIAL_DATA_RATE 9600


#pragma pack(push, 1)

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
#define UNIT_TYPE 1;

/// Can be edited end

/// RF message counter
uint32_t lastRfIndex { 0 };
/// LoRa message counter
uint8_t loraCounter { 0 };
/// Indication of first received message
bool firstRfMessage { true };

/**
 * Calculate 8b long checksum
**/
uint8_t checkSum8b(const uint8_t *data, size_t dataSize);

/**
 * Calculate 32b long checksum
**/
uint32_t checkSum32b(const uint8_t *data, size_t dataSize);

/**
 * Send LoRa message from given RF message
**/
void sendLoRaMessage(const RFMessage

&rfMessage);


/// Lora board instance
SoftwareSerial loraSerial(

LORA_TX_PIN, LORA_RX_PIN);

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


	//433MHZ RF communication
	// Set the type of RF communication
	vw_set_ptt_inverted(true);
	// Set bps speed
	vw_setup(RF_BITS_PER_SECOND);
	// Set pin number of the receiver
	vw_set_rx_pin(RF_WIRELESS_PIN);
	// Start the communication
	vw_rx_start();


}

void loop() {
	RFMessage rfMessage;
	uint8_t rfMessageSize = sizeof(RFMessage);

	/// Wait for RF message
	if(vw_get_message((uint8_t*) & rfMessage, &rfMessageSize)) {
		sendLoRaMessage(rfMessage);
	}
}

void sendLoRaMessage(const RFMessage &rfMessage) {
	LoRaMessage loraMessage;
	loraMessage.protocolVersion = PROTOCOL_VERSION;
	loraMessage.unitNumber = UNIT_NUMBER;
  loraMessage.unitType = UNIT_TYPE;
	loraMessage.flags = 0;
	/// Adding message counter to flags
	loraMessage.flags = loraCounter & 0b00001111;

	uint32_t calculatedCheckSum = checkSum32b((uint8_t*) & rfMessage, sizeof(RFMessage) - sizeof(uint32_t));
	if(calculatedCheckSum != rfMessage.checkSum) { /// RF message checksum error, values wont be copied
		Serial.println("Wrong RF message check Sum");
		loraMessage.flags |= 0b1000000;
	} else {  /// Checksum ok, copy values
		memcpy((void *)loraMessage.values, (void *)&rfMessage.temperature, 12);
		loraMessage.values[3] = 0;
		loraMessage.values[4] = 0;
	}

	/// Check if RF message got lost
	if(lastRfIndex + 1 != rfMessage.counter) {
	/// First message of program, do not set lost flag
		if(firstRfMessage) {
			firstRfMessage = false;
		} else {
			loraMessage.flags |= 0b01000000;
		}
	}
	lastRfIndex = rfMessage.counter;

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

uint32_t checkSum32b(const uint8_t *data, size_t dataSize) {
	uint32_t sum = 0;
	for(int index = 0; index < dataSize; index++) {
		sum += data[index];
	}
	return sum;
}

uint8_t checkSum8b(const uint8_t *data, size_t dataSize) {
	uint8_t sum = 0;
	for(int index = 0; index < dataSize; index++) {
		sum += data[index];
	}
	return sum;
}
