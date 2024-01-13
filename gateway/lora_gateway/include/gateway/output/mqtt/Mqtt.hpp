#pragma once

#include <gateway/output/Output.hpp>



namespace gateway::output::mqtt {
/**
 * @brief Output data to MQTT broker

 */
class Mqtt final: public Output {
public:
	/**
	 * @brief Construct a new Mqtt object
	 * @param context context of the gateway
	 */
	explicit Mqtt(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {};

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
};
}