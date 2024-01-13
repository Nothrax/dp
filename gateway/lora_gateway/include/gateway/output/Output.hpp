#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/device/Device.hpp>


#include <memory>



namespace gateway::output {
/**
 * @brief Output interface
 *
 */
class Output {
public:
	/**
	 * @brief Construct a new Output object
	 * @param context context of the gateway
	 */
	explicit Output(const std::shared_ptr<structures::GlobalContext> &context): context_(context) {};

	/**
	 * @brief Initialize the output
	 * @return true if successful
	 * @return false if failed
	 */
	virtual bool initialize() = 0;

	/**
	 * @brief Write data from device to the output
	 * @param device device to write data from
	 * @return true if successful
	 * @return false if failed
	 */
	virtual bool writeFromDevice(const std::shared_ptr<device::Device> &device) = 0;

	/**
	 * @brief Destroy the Output object
	 */
	virtual ~Output() = default;

protected:
	/// Context of the gateway
	std::shared_ptr<structures::GlobalContext> context_;
	/// Is the output initialized
	bool initialized_ { false };
};
}