#pragma once

#include <gateway/endpoint/Endpoint.hpp>



namespace gateway::endpoint::generator {
/**
 * @brief Fake endpoint, endpoint class with no connection, generates predefined messages with predefined timeout
 */
class GeneratorEndpoint final: public Endpoint {
public:
	/**
	 * @brief Construct a new Generator Endpoint object
	 * @param context context of the gateway
	 */
	explicit GeneratorEndpoint(const std::shared_ptr<structures::GlobalContext> &context): Endpoint(context) {};

	/**
	 * @brief Initialize the endpoint
	 * @return true if the initialization was successful
	 */
	bool initialize() override;

	/**
	 * @brief Get the Message object, generator endpoint will generate predefined message in the middle of the timeout
	 * throws if instance was not initialized
	 * @param timeoutMs timeout in milliseconds
	 * @return generated message
	 */
	[[nodiscard]] std::shared_ptr<structures::DeviceMessage> getMessage(unsigned int timeoutMs) override;

private:
	/// Maximum index of the message
	static constexpr uint32_t MAX_MESSAGE_INDEX { 15 };
	/// Message counter - index of the current message
	uint8_t messageCounter_ { 0 };
	/// Temperature to be set in the message
	float temperature_ { 20.5f };
	/// Humidity to be set in the message
	float humidity_ { 50.5f };
	/// CO2 to be set in the message
	uint32_t co2_ { 656 };
};
}
