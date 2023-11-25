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
		uint32_t entryCounter_ { 0 };
		std::ofstream file_;
	};
public:
	explicit CsvOutput(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {};

	bool initialize() override;

	bool writeFromDevice(const std::shared_ptr<device::Device> &device) override;

	~CsvOutput() override;

private:
	std::map<structures::EDeviceType, OutputEntry> outputsEntries_;

	void openFile(structures::EDeviceType deviceType);

	[[nodiscard]] bool isNewFile(structures::EDeviceType deviceType) const;

};
}
