#include <gateway/output_protocol/csv/CsvOutput.hpp>
#include <gateway/common_tools/TimeTools.hpp>

#include <iostream>



namespace gateway::output_protocol::csv {

bool CsvOutput::initialize() {
	initialized_ = true;
	return true;
}

bool CsvOutput::writeMessage(const std::shared_ptr<device::Message> &message) {
	if(!initialized_) {
		return false;
	}

	if(isNewFile(message->getDeviceType())) {
		openFile(message->getDeviceType());
		outputsEntries_.at(message->getDeviceType()).file_ << message->getCsvHeader();
	}

	outputsEntries_.at(message->getDeviceType()).file_ << message->getCsvEntry();
	outputsEntries_.at(message->getDeviceType()).entryCounter_++;
	return true;
}

void CsvOutput::openFile(input_protocol::EDeviceType deviceType) {
	if(outputsEntries_.count(deviceType) == 0) {
		outputsEntries_.insert(std::pair<input_protocol::EDeviceType, OutputEntry>(deviceType, OutputEntry()));
	} else {
		if(outputsEntries_.at(deviceType).file_.is_open()) {
			outputsEntries_.at(deviceType).file_.close();
		}
	}

	auto filename = "device_" + common_tools::EnumTools::enumToString(deviceType) + "_" +
					std::to_string(common_tools::TimeTools::getUnixTimestampS()) + ".csv";
	outputsEntries_.at(deviceType).entryCounter_ = 0;
	outputsEntries_.at(deviceType).file_.open(context_->settings->getCsvPath()/filename, std::ios::out | std::ios::app);
}

bool CsvOutput::isNewFile(input_protocol::EDeviceType deviceType) const {
	if(outputsEntries_.count(deviceType) == 0) {
		return true;
	}

	if(outputsEntries_.at(deviceType).entryCounter_ >= context_->settings->getNumberOfCsvEntries()) {
		return true;
	}

	return false;
}

CsvOutput::~CsvOutput() {
	for(auto &[_, outputEntry]: outputsEntries_) {
		if(outputEntry.file_.is_open()) {
			outputEntry.file_.close();
		}
	}
}
}
