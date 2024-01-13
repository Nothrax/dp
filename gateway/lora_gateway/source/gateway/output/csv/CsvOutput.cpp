#include <gateway/output/csv/CsvOutput.hpp>
#include <gateway/common_tools/TimeTools.hpp>

#include <iostream>



namespace gateway::output::csv {

bool CsvOutput::initialize() {
	initialized_ = true;
	return true;
}

bool CsvOutput::writeFromDevice(const std::shared_ptr<device::Device> &device) {
	if(!initialized_) {
		return false;
	}

	if(isNewFile(device->getDeviceType())) {
		openFile(device->getDeviceType());
		outputsEntries_.at(device->getDeviceType()).file_ << device->getCsvHeader();
	}

	outputsEntries_.at(device->getDeviceType()).file_ << device->getCsvEntry();
	outputsEntries_.at(device->getDeviceType()).entryCounter_++;
	return true;
}

void CsvOutput::openFile(structures::EDeviceType deviceType) {
	if(outputsEntries_.count(deviceType) == 0) {
		outputsEntries_.insert(std::pair<structures::EDeviceType, OutputEntry>(deviceType, OutputEntry()));
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

bool CsvOutput::isNewFile(structures::EDeviceType deviceType) const {
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
