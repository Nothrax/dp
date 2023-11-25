#pragma once

#include <gateway/output/Output.hpp>

#include <memory>



namespace gateway::output {
class OutputFactory {
public:
	/**
	 * @brief Create an output based on the device type
	 * @param context context of the gateway
	 * @return created output or nullptr if the device type is not supported
	 */
	static std::shared_ptr<Output> createOutput(const std::shared_ptr<structures::GlobalContext> &context);
};
}