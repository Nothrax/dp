#include <gateway/output/csv/CsvOutput.hpp>

#include <cstring>



namespace gateway::output::csv {
void CsvOutput::write(const gateway::structures::DeviceMessage &data) {
	if(!file_.is_open()) {
		openFile();
	}

	const auto p1 = std::chrono::system_clock::now();
	float floatValues[2];
	std::memcpy((void *)floatValues, (void *)data.values, 8);
	file_ << std::chrono::duration_cast<std::chrono::milliseconds>(
			p1.time_since_epoch()).count() << "," << std::to_string(data.protocolVersion) << ","
		  << std::to_string(data.unitNumber) << "," << std::to_string(data.flags) << "," << floatValues[0] << ","
		  << floatValues[1] << "," << data.values[2] << "," << std::to_string(data.checkSum) << "\n";

	entryCounter_++;
	if(entryCounter_ >= maxEntries_) {
		file_.close();
		entryCounter_ = 0;
	}
}

CsvOutput::~CsvOutput() {
	if(file_.is_open()) {
		file_.close();
	}
}

void CsvOutput::openFile() {
	auto filename = "data_" + std::to_string(std::time(0)) + ".csv";
	file_.open(filePath_/filename, std::ios::out | std::ios::app);
	file_ << "timestamp_ms,protocolVersion,unitNumber,flags,temp,hum,co2,checkSum\n";
}
}
