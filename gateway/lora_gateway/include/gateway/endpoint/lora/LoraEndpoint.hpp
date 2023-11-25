#pragma once

#include <gateway/endpoint/Endpoint.hpp>
#include <gateway/structures/DeviceMessage.hpp>
#include <gateway/endpoint/lora/UartManager.hpp>
#include <gateway/structures/GlobalContext.hpp>


#include <memory>



namespace gateway::endpoint::lora {
enum class ELoraMode {
	E_BEGIN = 0,
	E_INVALID = E_BEGIN,
	E_NORMAL_MODE,
	E_WAKE_UP_MODE,
	E_POWER_SAVING_MODE,
	E_SETTING_MODE,
	E_END
};

class LoraEndpoint final: public Endpoint {
public:
	explicit LoraEndpoint(const std::shared_ptr<structures::GlobalContext> &context): Endpoint(context) {};

	bool initialize() override;

	[[nodiscard]] std::shared_ptr<structures::DeviceMessage> getMessage(unsigned int timeoutMs) const override;

	~LoraEndpoint() override;

private:
	//todo pins from config ?
	/// aka BCM pin 23
	static constexpr uint32_t M0_PIN { 4 };
	/// aka BCM pin 24
	static constexpr uint32_t M1_PIN { 5 };

	static constexpr uint32_t LOW { 0 };
	static constexpr uint32_t HIGH { 1 };

	std::unique_ptr<UARTManager> uart_ { nullptr };
	//todo naplnit z konstruktoru?
	uint8_t settingsRegister_[6] { 0xc0, 0x14, 0x24, 0x1a, 0x02, 0xa4 };

	//todo nastaveni jednotlivych parametru?
	bool initializeUart();

	bool initializeModePins();

	void setMode(ELoraMode mode);

	bool initializeLora();
};
}