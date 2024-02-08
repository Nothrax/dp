#include <gateway/output_protocol/CsvManager.hpp>
#include <gateway/common_tools/EnumTools.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/device/scale/ScaleMessage.hpp>
#include <gateway/device/cellar/CellarMessage.hpp>

#include <iostream>



namespace gateway::output_protocol {

bool CsvManager::initialize() {
	for(const auto &supportedDevice: context_->settings->getSupportedDevices()) {
		auto deviceType = common_tools::EnumTools::valueToEnum<input_protocol::EDeviceType, uint32_t>(
				supportedDevice.deviceType);
		outputsEntries_[deviceType][supportedDevice.deviceNumber].entryCounter_ = 0;
		initializeDeviceEntry(deviceType, supportedDevice.deviceNumber);
		logger::Logger::logInfo("Initialised csv for device type {}, device number {}, number of entries {}.",
								common_tools::EnumTools::enumToString(deviceType), supportedDevice.deviceNumber,
								outputsEntries_[deviceType][supportedDevice.deviceNumber].entryCounter_);
	}
	return true;
}

bool CsvManager::storeMessage(const std::shared_ptr<device::Message> &message) {
	std::scoped_lock lock(outputsEntries_.at(message->getDeviceType()).at(message->getDeviceNumber()).fileMutex_);
	outputsEntries_.at(message->getDeviceType()).at(message->getDeviceNumber()).file_ << message->getCsvEntry();
	outputsEntries_.at(message->getDeviceType()).at(message->getDeviceNumber()).entryCounter_++;
	return true;
}

void CsvManager::initializeDeviceEntry(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) {
	std::scoped_lock lock(outputsEntries_.at(deviceType).at(deviceNumber).fileMutex_);
	auto deviceFile = getFilePath(deviceType, deviceNumber);
	if(std::filesystem::exists(deviceFile)) {
		std::ifstream inFile(deviceFile);
		size_t lineCount = std::count(
				std::istreambuf_iterator<char>(inFile),
				std::istreambuf_iterator<char>(),
				'\n'
		);
		outputsEntries_[deviceType][deviceNumber].entryCounter_ = lineCount - 1;
		inFile.close();
		outputsEntries_[deviceType][deviceNumber].file_.open(deviceFile, std::ios::out | std::ios::app);
	} else {
		outputsEntries_[deviceType][deviceNumber].entryCounter_ = 0;
		outputsEntries_[deviceType][deviceNumber].file_.open(deviceFile, std::ios::out | std::ios::app);
		switch(deviceType) {
			case input_protocol::EDeviceType::E_WINE_CELLAR:
				outputsEntries_[deviceType][deviceNumber].file_ << device::cellar::CellarMessage::getCsvHeader();
				break;
			case input_protocol::EDeviceType::E_BEE_SCALE:
				outputsEntries_[deviceType][deviceNumber].file_ << device::scale::ScaleMessage::getCsvHeader();
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

std::vector<std::shared_ptr<device::Message>>
CsvManager::getStoredMessages(input_protocol::EDeviceType deviceType, uint32_t deviceNumber) {
	std::scoped_lock lock(outputsEntries_.at(deviceType).at(deviceNumber).fileMutex_);
	//todo
	return {};
}
}
