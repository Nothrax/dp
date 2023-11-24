#pragma once

#include <gateway/structures/DeviceMessage.hpp>

#include <filesystem>
#include <fstream>

//todo common writer class
namespace gateway::cloud {
class CsvWriter {
public:
	explicit CsvWriter(const std::filesystem::path &filePath): filePath_(filePath){};

	void write(const structures::DeviceMessage &data);

	~CsvWriter();
private:
	std::filesystem::path filePath_;
	uint32_t entryCounter_ { 0 };
	uint32_t maxEntries_ { 1000 };
	std::ofstream file_;

	void openFile();

};
}
