#include <gateway/endpoint/lora/LoraEndpoint.hpp>
#include <gateway/logger/Logger.hpp>

#include <wiringPi.h>

#include <chrono>
#include <thread>



namespace gateway::endpoint::lora {
bool LoraEndpoint::initialize() {
	//todo flush uart buffer?
	if(!initializeUart()) {
		logger::Logger::logError("Failed to initialize UART");
		return false;
	}
	if(!initializeModePins()) {
		logger::Logger::logError("Failed to initialize LoRa board");
		return false;
	}
	if(!initializeLora()) {
		logger::Logger::logError("Failed to initialize LoRa board");
		return false;
	}
	initialized_ = true;
	return true;
}

std::shared_ptr<structures::DeviceMessage> LoraEndpoint::getMessage(unsigned int timeoutMs) const {
	std::shared_ptr<structures::DeviceMessage> message;
	if(!initialized_) {
		throw std::runtime_error("LoRa endpoint was not initialized");
	}
	uint8_t receiveBuffer[sizeof(structures::DeviceMessage)];

	//todo timeout
	uart_->readBuffer(receiveBuffer, sizeof(receiveBuffer));
	message = std::make_shared<structures::DeviceMessage>();
	//todo correct?
	memcpy((void *)message.get(), (void *)receiveBuffer, sizeof(receiveBuffer));

	return message;
}

LoraEndpoint::~LoraEndpoint() {
	//todo deinit lora?
}

bool LoraEndpoint::initializeModePins() {
	if(wiringPiSetup() == -1) {
		return false;
	}
	pinMode(M0_PIN, OUTPUT);
	pinMode(M1_PIN, OUTPUT);
	return true;
}

bool LoraEndpoint::initializeUart() {
	uart_ = std::make_unique<UARTManager>();
	return uart_->startUart(context_->settings->getUartDevice(), context_->settings->getBaudRate());
}

void LoraEndpoint::setMode(ELoraMode mode) {
	//check datasheet for E32-868T20D for more info
	switch(mode) {
		case ELoraMode::E_NORMAL_MODE:
			digitalWrite(M0_PIN, LOW);
			digitalWrite(M1_PIN, LOW);
			break;
		case ELoraMode::E_WAKE_UP_MODE:
			digitalWrite(M0_PIN, HIGH);
			digitalWrite(M1_PIN, LOW);
			break;
		case ELoraMode::E_POWER_SAVING_MODE:
			digitalWrite(M0_PIN, LOW);
			digitalWrite(M1_PIN, HIGH);
			break;
		case ELoraMode::E_SETTING_MODE:
			digitalWrite(M0_PIN, HIGH);
			digitalWrite(M1_PIN, HIGH);
			break;
		default:
			logger::Logger::logWarning("Trying to set non-existing mode, continuing without changing mode.");
			break;
	}
}

bool LoraEndpoint::initializeLora() {
	uint8_t receiveRegister[6];
	/// Change lora mode to sleep mode that allows sending setting command
	setMode(ELoraMode::E_SETTING_MODE);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	/// Send register with all parameters
	uart_->sendBuffer(settingsRegister_, sizeof(settingsRegister_));
	uart_->readBuffer(receiveRegister, sizeof(receiveRegister));

	for(int i = 0; i < sizeof(settingsRegister_); i++) {
		if(settingsRegister_[i] != receiveRegister[i]) {
			return false;
		}
	}

	//todo check the set values?

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	/// Set mode back to wireless communication
	setMode(ELoraMode::E_NORMAL_MODE);
	return true;
}
}