#include <gateway/cloud/CsvWriter.hpp>

#include <cstring>

namespace gateway::cloud {
void CsvWriter::write(const gateway::structures::LoraMessage &data) {
	if(!file_.is_open()){
		openFile();
	}

	float floatValues[2];
	std::memcpy((void *)floatValues, (void *) data.values, 8);
	file_ << std::to_string(data.protocolVersion) << "," << std::to_string(data.unitNumber) << "," << data.flags << "," << floatValues[0] << "," << floatValues[1] << "," << data.values[2] << "," << std::to_string(data.checkSum) << "\n";

	entryCounter_++;
	if(entryCounter_ >= maxEntries_){
		file_.close();
		entryCounter_ = 0;
	}
}

CsvWriter::~CsvWriter() {
	if(file_.is_open()){
		file_.close();
	}
}

void CsvWriter::openFile() {
	auto filename = "data_" + std::to_string(std::time(0)) +".csv";
	file_.open(filename, std::ios::out | std::ios::app);
	file_ << "protocolVersion,unitNumber,flags,temp,hum,co2,checkSum\n";
}
}
