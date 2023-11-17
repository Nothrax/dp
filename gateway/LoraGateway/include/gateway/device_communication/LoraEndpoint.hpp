#pragma once

#include <gateway/structures/LoraMessage.hpp>
#include <gateway/device_communication/UartManager.hpp>
#include <gateway/structures/GlobalContext.hpp>

#include <wiringPi.h>

#include <memory>



namespace gateway::device_communication {
enum class ELoraMode {
	E_BEGIN = 0,
	E_INVALID = E_BEGIN,
	E_NORMAL_MODE,
	E_WAKE_UP_MODE,
	E_POWER_SAVING_MODE,
	E_SETTING_MODE,
	E_END
};

class LoraEndpoint {
public:
	explicit LoraEndpoint(std::shared_ptr<structures::GlobalContext> &context): context_(context) {};

	bool initialize();

	structures::LoraMessage getMessage();

	~LoraEndpoint();

private:
	//todo pins from config ?
	/// aka BCM pin 23
	static constexpr uint32_t M0_PIN { 4 };
	/// aka BCM pin 24
	static constexpr uint32_t M1_PIN { 5 };

	static constexpr uint32_t LOW { 0 };
	static constexpr uint32_t HIGH { 1 };

	std::shared_ptr<structures::GlobalContext> &context_;
	std::unique_ptr<UART> uart_ { nullptr };
	//todo naplnit z konstruktoru?
	uint8_t settingsRegister_[6] { 0xc0, 0x14, 0x24, 0x1a, 0x02, 0xa4 };
	uint8_t receiveBuffer_[sizeof(structures::LoraMessage)];

	//todo nastaveni jednotlivych parametru?
	bool initializeUart();

	bool initializeModePins();

	void setMode(ELoraMode mode);

	bool initializeLora();
};
}