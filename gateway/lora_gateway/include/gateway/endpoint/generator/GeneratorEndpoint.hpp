#pragma once

#include <gateway/endpoint/Endpoint.hpp>



namespace gateway::endpoint::generator{
/**
 * @brief Fake endpoint, endpoint class with no connection, generates predefined messages
 */
class GeneratorEndpoint final: public Endpoint {
public:
	explicit GeneratorEndpoint(const std::shared_ptr<structures::GlobalContext> &context): Endpoint(context) {};

	bool initialize() override;

	[[nodiscard]] std::shared_ptr<structures::DeviceMessage> getMessage(unsigned int timeoutMs) const override;

	~GeneratorEndpoint() override = default;

private:

};
}
