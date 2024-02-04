#pragma once

#include <gateway/output_protocol/Output.hpp>

#include <filesystem>
#include <fstream>
#include <map>



namespace gateway::output_protocol::csv {
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
	 * @param message message to write
	 * @return true if the write was successful
	 */
	bool writeMessage(const std::shared_ptr<device::Message> &message) override;

	~CsvOutput() override;

private:
	/// Map of output entries, one entry for each device type
	std::map<input_protocol::EDeviceType, OutputEntry> outputsEntries_;

	void openFile(input_protocol::EDeviceType deviceType);

	[[nodiscard]] bool isNewFile(input_protocol::EDeviceType deviceType) const;

};
}
