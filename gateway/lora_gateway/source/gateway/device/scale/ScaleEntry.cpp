#include <gateway/device/scale/ScaleEntry.hpp>

#include <sstream>



namespace gateway::device::scale {
std::string ScaleEntry::getCsvHeader() {
	return "timestamp_ms,flags,temperature,humidity,weight\n";
}

std::string ScaleEntry::getCsvEntry() const {
	return std::to_string(currentTimestampMs_) + "," + std::to_string(flags_) + "," + std::to_string(temperature_) +
		   "," + std::to_string(humidity_) + "," + std::to_string(weight_) + "\n";
}

boost::json::object ScaleEntry::getOutputProtocolEntry() const {
	boost::json::object obj {
			{ "timestamp_ms", currentTimestampMs_ },
			{ "flags",        flags_ },
			{ "temperature",  temperature_ },
			{ "humidity",     humidity_ },
			{ "weight",       weight_ }
	};
	return obj;
}

void ScaleEntry::setWeight(float weight) {
	weight_ = weight;
}

void ScaleEntry::setTemperature(float temperature) {
	temperature_ = temperature;
}

void ScaleEntry::setHumidity(float humidity) {
	humidity_ = humidity;
}

void ScaleEntry::setFromCsvEntry(const std::string &csvEntry) {
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
	weight_ = std::stof(values[4]);
}

}
