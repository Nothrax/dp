#pragma once

#include <gateway/output/Output.hpp>

#include <mqtt/async_client.h>

#include <memory>



namespace gateway::output::mqtt_protocol {
/**
 * @brief Output data to MQTT broker

 */
class Mqtt final: public Output {
public:
	/**
	 * @brief Construct a new Mqtt object
	 * @param context context of the gateway
	 */
	explicit Mqtt(const std::shared_ptr<structures::GlobalContext> &context);

	/**
	 * @brief Initialize the output, must be called before writeFromDevice
	 * @return true if the initialization was successful
	 * @return false if the initialization failed
	 */
	bool initialize() override;

	/**
	 * @brief Publish to device topic
	 * @param device device to publish from
	 * @return true if the publish was successful
	 */
	bool writeFromDevice(const std::shared_ptr<device::Device> &device) override;
private:
	static constexpr int QOS{0};
	std::unique_ptr<mqtt::async_client> client_ { nullptr };
	std::string publishTopic_;
	std::string serverAddress_;


	bool connect();
	void disconnect();
};
}