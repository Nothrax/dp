#pragma once

#include <gateway/input_protocol/Endpoint.hpp>



namespace gateway::input_protocol::generator {
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
	[[nodiscard]] std::shared_ptr<input_protocol::InputProtocolMessage> getMessage(unsigned int timeoutMs) override;

private:
	static constexpr float MAX_TEMPERATURE { 70.0f };
	static constexpr float MIN_TEMPERATURE_ { -20.0f };
	static constexpr float MAX_HUMIDITY { 100.0f };
	static constexpr float MIN_HUMIDITY { 0.0f };
	static constexpr uint32_t MAX_CO2 { 10000 };
	static constexpr uint32_t MIN_CO2 { 10000 };
	static constexpr float MAX_WEIGHT { 200.0f };
	static constexpr float MIN_WEIGHT { 0.0f };
	static constexpr float TEMPERATURE_STEP { 0.5f };
	static constexpr float HUMIDITY_STEP { 0.5f };
	static constexpr uint32_t CO2_STEP { 10 };
	static constexpr float WEIGHT_STEP { 0.5f };


	/// Maximum index of the message
	static constexpr uint32_t MAX_MESSAGE_INDEX { 15 };
	/// Message counter - index of the current message
	uint8_t messageCounter_ { 0 };
	/// Temperature to be set in the message
	float temperatureCellar_ { 20.5f };
	/// Humidity to be set in the message
	float humidityCellar_ { 50.5f };
	/// CO2 to be set in the message
	uint32_t co2Cellar_ { 656 };

	float humidityBee_ { 80.5f };
	float temperatureBee_ { 15.5f };
	float weightBee_ { 109.5f };


	void updateValues();
};
}
