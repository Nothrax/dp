#include <gateway/output_protocol/CsvManager.hpp>
#include <gateway/common_tools/EnumTools.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/device/scale/ScaleEntry.hpp>
#include <gateway/device/cellar/CellarEntry.hpp>

#include <iostream>



namespace gateway::output_protocol {

bool CsvManager::initialize() {
	for(const auto &supportedDevice: context_->settings->getSupportedDevices()) {
		auto deviceType = common_tools::EnumTools::valueToEnum<input_protocol::EDeviceType, uint32_t>(
				supportedDevice.deviceType);
		outputsEntries_[deviceType][supportedDevice.deviceNumber].entryCounter_ = 0;
		std::scoped_lock lock(outputsEntries_.at(deviceType).at(supportedDevice.deviceNumber).fileMutex_);
		initializeDeviceEntry(deviceType, supportedDevice.deviceNumber);
		logger::Logger::logInfo("Initialised csv for device type {}, device number {}, number of entries {}.",
								common_tools::EnumTools::enumToString(deviceType), supportedDevice.deviceNumber,
								outputsEntries_[deviceType][supportedDevice.deviceNumber].entryCounter_);
	}
	return true;
}

void CsvManager::storeMessage(const std::shared_ptr<device::Entry> &message) {
	std::scoped_lock lock(outputsEntries_.at(message->getDeviceType()).at(message->getDeviceNumber()).fileMutex_);
	outputsEntries_.at(message->getDeviceType()).at(message->getDeviceNumber()).file_ << message->getCsvEntry();
	outputsEntries_.at(message->getDeviceType()).at(message->getDeviceNumber()).entryCounter_++;
}

void CsvManager::initializeDeviceEntry(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) {
	auto deviceFile = getFilePath(deviceType, deviceNumber);
	if(std::filesystem::exists(deviceFile)) {
		std::ifstream inFile(deviceFile);
		size_t lineCount = std::count(
				std::istreambuf_iterator<char>(inFile),
				std::istreambuf_iterator<char>(),
				'\n'
		);
		if(lineCount > 0){
			outputsEntries_[deviceType][deviceNumber].entryCounter_ = lineCount - 1;
		}else{
			outputsEntries_[deviceType][deviceNumber].entryCounter_ = 0;
		}
		inFile.close();
		outputsEntries_[deviceType][deviceNumber].file_.open(deviceFile, std::ios::out | std::ios::app);
	} else {
		outputsEntries_[deviceType][deviceNumber].entryCounter_ = 0;
		outputsEntries_[deviceType][deviceNumber].file_.open(deviceFile, std::ios::out | std::ofstream::trunc);
		switch(deviceType) {
			case input_protocol::EDeviceType::E_WINE_CELLAR:
				outputsEntries_[deviceType][deviceNumber].file_ << device::cellar::CellarEntry::getCsvHeader();
				break;
			case input_protocol::EDeviceType::E_BEE_SCALE:
				outputsEntries_[deviceType][deviceNumber].file_ << device::scale::ScaleEntry::getCsvHeader();
				break;
		}
	}

}

CsvManager::~CsvManager() {
	for(auto &[_, devices]: outputsEntries_) {
		for(auto &[_, outputEntry]: devices) {
			if(outputEntry.file_.is_open()) {
				outputEntry.file_.close();
			}
		}
	}
}

bool CsvManager::areStoredMessages(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) {
	std::scoped_lock lock(outputsEntries_.at(deviceType).at(deviceNumber).fileMutex_);
	return outputsEntries_.at(deviceType).at(deviceNumber).entryCounter_ > 0;
}

std::filesystem::path CsvManager::getFilePath(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) const {
	std::string filename;
	filename = std::to_string(static_cast<uint32_t>(deviceType)) + "_" + std::to_string(deviceNumber) + ".csv";
	return context_->settings->getCsvPath()/filename;
}

std::vector<std::shared_ptr<device::Entry>>
CsvManager::getStoredMessages(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) {
	std::scoped_lock lock(outputsEntries_.at(deviceType).at(deviceNumber).fileMutex_);
	std::vector<std::shared_ptr<device::Entry>> entries;

	if(outputsEntries_.at(deviceType).at(deviceNumber).file_.is_open()) {
		outputsEntries_.at(deviceType).at(deviceNumber).file_.close();
	}
	auto file = std::ifstream { getFilePath(deviceType, deviceNumber) };
	std::string line;
	///read first line with header
	std::getline(file, line);

	while(std::getline(file, line)) {
		std::shared_ptr<device::Entry> entry;
		if(deviceType == input_protocol::EDeviceType::E_BEE_SCALE) {
			entry = std::make_shared<device::scale::ScaleEntry>(deviceNumber);
		} else if(deviceType == input_protocol::EDeviceType::E_WINE_CELLAR) {
			entry = std::make_shared<device::cellar::CellarEntry>(deviceNumber);
		}
		entry->setFromCsvEntry(line);
		entries.push_back(entry);
	}
	/// All data read from the file, reset it
	file.close();
	std::remove(getFilePath(deviceType, deviceNumber).c_str());
	initializeDeviceEntry(deviceType, deviceNumber);

	return entries;
}

void CsvManager::storeMessages(const std::vector<std::shared_ptr<device::Entry>> &message) {
	for(const auto &msg: message) {
		storeMessage(msg);
	}
}
}
