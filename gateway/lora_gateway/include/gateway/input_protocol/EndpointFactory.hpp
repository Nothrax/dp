#pragma once

#include <gateway/settings/Settings.hpp>
#include <gateway/input_protocol/Endpoint.hpp>

#include <memory>



namespace gateway::input_protocol {
class EndpointFactory {
public:
	/**
	 * @brief Create an endpoint based on the device type
	 * @param context context of the gateway
	 * @return created endpoint or nullptr if the device type is not supported
	 */
	static std::shared_ptr<Endpoint> createEndpoint(const std::shared_ptr<structures::GlobalContext> &context);
};
}