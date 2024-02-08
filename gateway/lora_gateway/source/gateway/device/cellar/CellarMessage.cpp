#include <gateway/device/cellar/CellarMessage.hpp>



namespace gateway::device::cellar {
std::string CellarMessage::getCsvHeader() {
	return "timestamp_ms,flags,temperature,humidity,co2\n";
}

std::string CellarMessage::getCsvEntry() const {
	return std::to_string(currentTimestampMs_) + "," + std::to_string(flags_) + "," + std::to_string(temperature_) +
		   "," + std::to_string(humidity_) + "," + std::to_string(co2_) + "\n";
}

boost::json::object CellarMessage::getOutputProtocolEntry() const {
	boost::json::object obj {
			{ "timestamp_ms", currentTimestampMs_ },
			{ "flags",        flags_ },
			{ "temperature",  temperature_ },
			{ "humidity",     humidity_ },
			{ "co2",          co2_ }
	};

	return obj;
}

void CellarMessage::setCo2(uint32_t co2) {
	co2_ = co2;
}

void CellarMessage::setTemperature(float temperature) {
	temperature_ = temperature;
}

void CellarMessage::setHumidity(float humidity) {
	humidity_ = humidity;
}
}
