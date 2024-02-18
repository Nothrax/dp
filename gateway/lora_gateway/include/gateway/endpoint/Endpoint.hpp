#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/structures/DeviceMessage.hpp>

#include <memory>



namespace gateway::endpoint {
/**
 * @brief Base class for all endpoints
 */
class Endpoint {
public:
	/**
	 * @brief Construct a new Endpoint object
	 * @param context context of the gateway
	 */
	explicit Endpoint(const std::shared_ptr<structures::GlobalContext> &context): context_(context) {};

	/**
	 * @brief Initialize the endpoint
	 * @return true if the initialization was successful
	 * @return false if the initialization failed
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Get the Message object, blocks until a message is received or timeout occurs
	 * @param timeout timeout in milliseconds
	 * @return received message or nullptr if timeout occurred
	 */
	[[nodiscard]] virtual std::shared_ptr<structures::DeviceMessage> getMessage(unsigned int timeoutMs) = 0;

	/**
	 * @brief Destroy the Endpoint object
	 */
	virtual ~Endpoint() = default;

protected:
	std::shared_ptr<structures::GlobalContext> context_;
	/// Indication if the endpoint is initialized
	bool initialized_ { false };

};
}
