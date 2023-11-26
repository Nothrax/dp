#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/endpoint/Endpoint.hpp>
#include <gateway/output/Output.hpp>
#include <gateway/device/Device.hpp>

#include <cstdint>
#include <map>



namespace gateway {
/**
 * @brief Gateway class
 */
class Gateway {
public:
	explicit Gateway(const std::shared_ptr<structures::GlobalContext> &globalContext): context_(globalContext) {};

	/**
	 * @brief Start the gateway
	 */
	void start();

private:
	/// Global context of the gateway
	std::shared_ptr<structures::GlobalContext> context_;
	/// Endpoint of the gateway, receiving messages from devices
	std::shared_ptr<endpoint::Endpoint> endpoint_;
	/// Output of the gateway, outputting messages to desired output
	std::shared_ptr<output::Output> output_;
	/// Devices of the gateway mapped by type and number, each device has its own device::Device instance
	std::map<uint32_t, std::map<uint32_t, std::shared_ptr<device::Device>>> devices_;
	/// Timeout for receiving messages from devices
	static constexpr uint32_t messageReceiveTimeoutMs_ { 2000 };

	bool initialize();

	void processMessage(const structures::DeviceMessage &message);
};
}