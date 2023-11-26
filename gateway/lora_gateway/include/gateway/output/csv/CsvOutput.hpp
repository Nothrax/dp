#pragma once

#include <gateway/output/Output.hpp>

#include <filesystem>
#include <fstream>
#include <map>



namespace gateway::output::csv {
/**
 * @brief Output to a CSV file
 */
class CsvOutput final: public Output {
private:
	/**
	 * @brief Entry of a specific device type
	 */
	struct OutputEntry {
		/// Number of entries in current file
		uint32_t entryCounter_ { 0 };
		/// File to write to
		std::ofstream file_;
	};
public:
	/**
	 * @brief Constructor
	 * @param context context of the gateway
	 */
	explicit CsvOutput(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {};

	/**
	 * @brief Initialize the output, must be called before writeFromDevice
	 * @return
	 */
	bool initialize() override;

	/**
	 * @brief Write data from the device to the output file, when maximum number of entries is reached, a new file is created
	 * @param device device to write from
	 * @return true if the write was successful
	 */
	bool writeFromDevice(const std::shared_ptr<device::Device> &device) override;

	~CsvOutput() override;

private:
	/// Map of output entries, one entry for each device type
	std::map<structures::EDeviceType, OutputEntry> outputsEntries_;

	void openFile(structures::EDeviceType deviceType);

	[[nodiscard]] bool isNewFile(structures::EDeviceType deviceType) const;

};
}
