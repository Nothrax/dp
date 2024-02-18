#pragma once

#include <gateway/input_protocol/Endpoint.hpp>
#include <gateway/input_protocol/InputProtocolMessage.hpp>
#include <gateway/input_protocol/lora/UartManager.hpp>
#include <gateway/structures/GlobalContext.hpp>


#include <memory>



namespace gateway::input_protocol::lora {
/**
 * @brief Lora mode, see E32-868T20D datasheet for more information
 */
enum class ELoraMode {
	E_BEGIN = 0,
	E_INVALID = E_BEGIN,
	E_NORMAL_MODE,
	E_WAKE_UP_MODE,
	E_POWER_SAVING_MODE,
	E_SETTING_MODE,
	E_END
};

/**
 * @brief Lora endpoint, endpoint class for Lora communication, uses UART, board is E32-868T20D
 */
class LoraEndpoint final: public Endpoint {
public:
	explicit LoraEndpoint(const std::shared_ptr<structures::GlobalContext> &context);

	/**
	 * @brief Initialize the endpoint - set pins, start uart and set up the board
	 * @return true if the initialization was successful
	 * @return false if the initialization failed
	 */
	bool initialize() override;

	/**
	 * @brief Get the Message object, blocks until a message is received or timeout occurs
	 * throws if instance was not initialized
	 * @param timeoutMs timeout in milliseconds
	 * @return received message or nullptr if timeout occurred
	 */
	[[nodiscard]] std::shared_ptr<input_protocol::InputProtocolMessage> getMessage(unsigned int timeoutMs) override;

private:
	/// Wait for setup to take place in milliseconds
	static constexpr uint32_t SETUP_WAIT_MS { 1000 };
	/// Control command 0xc0 - save the parameters when power-down
	static constexpr uint8_t HEAD_COMMAND { 0xc0 };
	/// Sped register - 8N1 UART mode, 9600 UART baud rate, 2.4k Air Data Rate
	static constexpr uint8_t SPED_COMMAND { 0x1a };
	/// Option register - Transparent transition mode, IO drive mode 0, Wireles wake-up time 1250ms, FEC off, transmission power 30dBm
	static constexpr uint8_t OPTION_COMMAND { 0xa4 };

	/// Index of the head register
	static constexpr uint8_t HEAD_REGISTER_INDEX { 0 };
	/// Index of the high byte of address register
	static constexpr uint8_t HIGH_ADDRESS_REGISTER_INDEX { 1 };
	/// Index of the low byte of address register
	static constexpr uint8_t LOW_ADDRESS_REGISTER_INDEX { 2 };
	/// Index of the channel register
	static constexpr uint8_t SPED_REGISTER_INDEX { 3 };
	/// Index of the option register
	static constexpr uint8_t CHANNEL_REGISTER_INDEX { 4 };
	/// Index of the option register
	static constexpr uint8_t OPTION_REGISTER_INDEX { 5 };

	/// UART manager instance
	std::unique_ptr<UARTManager> uart_ { nullptr };
	/// Receive buffer
	std::vector<uint8_t> receiveBuffer_;

	bool initializeUart();

	bool initializeModePins();

	void setMode(ELoraMode mode);

	bool initializeLora();
};
}