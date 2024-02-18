#pragma once

#include <gateway/device/Entry.hpp>
#include <gateway/structures/GlobalContext.hpp>
#include <gateway/input_protocol/InputProtocolMessage.hpp>

#include <filesystem>
#include <fstream>
#include <map>



namespace gateway::output_protocol {
/**
 * @brief Entry of a specific device type
 */
struct OutputEntry {
	/// Number of entries in current file
	uint32_t entryCounter_ { 0 };
	/// File to write to
	std::ofstream file_;
	/// Mutex for file access
	std::mutex fileMutex_;
};
/**
 * @brief Output to a CSV file
 */
class CsvManager {
private:
public:
	/**
	 * @brief Constructor
	 * @param context context of the gateway
	 */
	explicit CsvManager(const std::shared_ptr<structures::GlobalContext> &context): context_(context) {};

	/**
	 * @brief Initialize the output, must be called before writeFromDevice
	 * @return
	 */
	bool initialize();

	bool areStoredMessages(input_protocol::EDeviceType deviceType, uint32_t deviceNumber);

	/**
	 * @brief Write data from the device to the output file, when maximum number of entries is reached, a new file is created
	 * @param message message to write
	 * @return true if the write was successful
	 */
	void storeMessage(const std::shared_ptr<device::Entry> &message);
	void storeMessages(const std::vector<std::shared_ptr<device::Entry>> &message);

	std::vector<std::shared_ptr<device::Entry>> getStoredMessages(input_protocol::EDeviceType deviceType, uint32_t deviceNumber);

	~CsvManager();

private:
	/// Map of output entries, one entry for each device type
	std::map<input_protocol::EDeviceType, std::map<uint32_t, OutputEntry>> outputsEntries_;
	std::shared_ptr<structures::GlobalContext> context_;

	void initializeDeviceEntry(input_protocol::EDeviceType deviceType, uint32_t deviceNumber);

	[[nodiscard]] std::filesystem::path getFilePath(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) const;

};
}
