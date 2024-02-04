#include <gateway/device/Cellar.hpp>

#include <cstring>

namespace gateway::device {

std::string Cellar::getCsvHeader() const {
	return "timestamp_ms,flags,temperature,humidity,co2\n";
}

std::string Cellar::getCsvEntry() const {
	return std::to_string(currentTimestampMs_) + "," + std::to_string(flags_) + "," + std::to_string(temperature_) +
		   "," + std::to_string(humidity_) + "," + std::to_string(co2_) + "\n";
}

boost::json::object Cellar::getOutputProtocolEntry() const {
	boost::json::object obj{
			{"timestamp_ms", currentTimestampMs_},
			{"flags", flags_},
			{"temperature", temperature_},
			{"humidity", humidity_},
			{"co2", co2_}
	};

	return obj;
}

bool Cellar::parseData(const structures::DeviceMessage &message) {
	std::memcpy((void*)&temperature_, (void *)&message.values[0], sizeof(float));
	std::memcpy((void*)&humidity_, (void *)&message.values[1], sizeof(float));
	co2_ = message.values[2];
	return true;
}
}