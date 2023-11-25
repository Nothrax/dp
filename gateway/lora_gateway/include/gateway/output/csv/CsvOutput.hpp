#pragma once

#include <gateway/structures/DeviceMessage.hpp>

#include <filesystem>
#include <fstream>

//todo common writer class
namespace gateway::output::csv{
class CsvOutput {
public:
	explicit CsvOutput(const std::filesystem::path &filePath, uint32_t maxEntries): filePath_(filePath), maxEntries_(maxEntries){};

	void write(const structures::DeviceMessage &data);

	~CsvOutput();
private:
	std::filesystem::path filePath_;
	uint32_t entryCounter_ { 0 };
	uint32_t maxEntries_ { 1000 };
	std::ofstream file_;

	void openFile();

};
}
