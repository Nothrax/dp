#include <gateway/endpoint/lora/LoraEndpoint.hpp>
#include <gateway/logger/Logger.hpp>

#include <wiringPi.h>

#include <chrono>
#include <thread>



namespace gateway::endpoint::lora {

LoraEndpoint::LoraEndpoint(const std::shared_ptr<structures::GlobalContext> &context): Endpoint(context) {
	receiveBuffer_ = std::vector<uint8_t>(sizeof(structures::DeviceMessage));
}

bool LoraEndpoint::initialize() {
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
	logger::Logger::logInfo("LoRa endpoint initialized");
	return true;
}

std::shared_ptr<structures::DeviceMessage> LoraEndpoint::getMessage(unsigned int timeoutMs) {
	std::shared_ptr<structures::DeviceMessage> message { nullptr };
	if(!initialized_) {
		throw std::runtime_error("LoRa endpoint was not initialized");
	}

	if(!uart_->readBuffer(receiveBuffer_, timeoutMs)) {
		return message;
	}
	message = std::make_shared<structures::DeviceMessage>();

	std::memcpy((void *)message.get(), (void *)receiveBuffer_.data(), receiveBuffer_.capacity());

	return message;
}

bool LoraEndpoint::initializeModePins() {
	if(wiringPiSetup() == -1) {
		return false;
	}
	pinMode(context_->settings->getM0Pin(), OUTPUT);
	pinMode(context_->settings->getM1Pin(), OUTPUT);
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
			digitalWrite(context_->settings->getM0Pin(), LOW);
			digitalWrite(context_->settings->getM1Pin(), LOW);
			break;
		case ELoraMode::E_WAKE_UP_MODE:
			digitalWrite(context_->settings->getM0Pin(), HIGH);
			digitalWrite(context_->settings->getM1Pin(), LOW);
			break;
		case ELoraMode::E_POWER_SAVING_MODE:
			digitalWrite(context_->settings->getM0Pin(), LOW);
			digitalWrite(context_->settings->getM1Pin(), HIGH);
			break;
		case ELoraMode::E_SETTING_MODE:
			digitalWrite(context_->settings->getM0Pin(), HIGH);
			digitalWrite(context_->settings->getM1Pin(), HIGH);
			break;
		default:
			logger::Logger::logWarning("Trying to set non-existing mode, continuing without changing mode.");
			break;
	}
}

bool LoraEndpoint::initializeLora() {
	std::vector<uint8_t> receiveRegister(6);
	/// Change lora mode to sleep mode that allows sending setting command
	setMode(ELoraMode::E_SETTING_MODE);

	std::vector<uint8_t> settingsRegister(6);
	settingsRegister[HEAD_REGISTER_INDEX] = HEAD_COMMAND;
	uint16_t loraAddress = context_->settings->getLoraAddress();
	settingsRegister[HIGH_ADDRESS_REGISTER_INDEX] = (loraAddress >> 8) & 0xff;
	settingsRegister[LOW_ADDRESS_REGISTER_INDEX] = loraAddress & 0xff;
	settingsRegister[SPED_REGISTER_INDEX] = SPED_COMMAND;
	settingsRegister[CHANNEL_REGISTER_INDEX] = context_->settings->getLoraChannel();
	settingsRegister[OPTION_REGISTER_INDEX] = OPTION_COMMAND;

	std::this_thread::sleep_for(std::chrono::milliseconds(SETUP_WAIT_MS));
	/// Send register with all parameters
	uart_->sendBuffer(settingsRegister);
	uart_->readBuffer(receiveRegister, SETUP_WAIT_MS);

	/// Check if the register was set correctly
	if(settingsRegister != receiveRegister) {
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	/// Set mode back to wireless communication
	setMode(ELoraMode::E_NORMAL_MODE);
	return true;
}
}