#include <gateway/device/Scale.hpp>

#include <boost/json.hpp>

#include <cstring>

namespace gateway::device {

std::string Scale::getCsvHeader() const {
	return "timestamp_ms,flags,temperature,humidity,weight\n";
}

std::string Scale::getCsvEntry() const {
	return std::to_string(currentTimestampMs_) + "," + std::to_string(flags_) + "," + std::to_string(temperature_) +
		   "," + std::to_string(humidity_) + "," + std::to_string(weight_) + "\n";
}

boost::json::object Scale::getOutputProtocolEntry() const {
	boost::json::object obj{
			{"timestamp_ms", currentTimestampMs_},
			{"flags", flags_},
			{"temperature", temperature_},
			{"humidity", humidity_},
			{"weight", weight_}
	};

	return obj;
}

bool Scale::parseData(const structures::DeviceMessage &message) {
	std::memcpy((void*)&temperature_, (void *)&message.values[0], sizeof(float));
	std::memcpy((void*)&humidity_, (void *)&message.values[1], sizeof(float));
	std::memcpy((void*)&weight_, (void *)&message.values[2], sizeof(float));
	return true;
}
}