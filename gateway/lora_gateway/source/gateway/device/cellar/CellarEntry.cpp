#include <gateway/device/cellar/CellarEntry.hpp>

#include <sstream>


namespace gateway::device::cellar {
std::string CellarEntry::getCsvHeader() {
	return "timestamp_ms,flags,temperature,humidity,co2\n";
}

std::string CellarEntry::getCsvEntry() const {
	return std::to_string(currentTimestampMs_) + "," + std::to_string(flags_) + "," + std::to_string(temperature_) +
		   "," + std::to_string(humidity_) + "," + std::to_string(co2_) + "\n";
}

boost::json::object CellarEntry::getOutputProtocolEntry() const {
	boost::json::object obj {
			{ "timestamp_ms", currentTimestampMs_ },
			{ "flags",        flags_ },
			{ "temperature",  temperature_ },
			{ "humidity",     humidity_ },
			{ "co2",          co2_ }
	};

	return obj;
}

void CellarEntry::setCo2(uint32_t co2) {
	co2_ = co2;
}

void CellarEntry::setTemperature(float temperature) {
	temperature_ = temperature;
}

void CellarEntry::setHumidity(float humidity) {
	humidity_ = humidity;
}

void CellarEntry::setFromCsvEntry(const std::string &csvEntry) {
	std::istringstream ss(csvEntry);
	std::string token;
	std::vector<std::string> values;
	while(std::getline(ss, token, ',')) {
		values.push_back(token);
	}

	currentTimestampMs_ = std::stoll(values[0]);
	flags_ = std::stoi(values[1]);
	temperature_ = std::stof(values[2]);
	humidity_ = std::stof(values[3]);
	co2_ = std::stoi(values[4]);
}
}
