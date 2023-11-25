#pragma once

#include <gateway/settings/Settings.hpp>
#include <gateway/endpoint/Endpoint.hpp>

#include <memory>



namespace gateway::endpoint {
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