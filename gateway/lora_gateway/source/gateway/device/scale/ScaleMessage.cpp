#include <gateway/device/scale/ScaleMessage.hpp>



namespace gateway::device::scale {
std::string ScaleMessage::getCsvHeader() const {
	return "timestamp_ms,flags,temperature,humidity,weight\n";
}

std::string ScaleMessage::getCsvEntry() const {
	return std::to_string(currentTimestampMs_) + "," + std::to_string(flags_) + "," + std::to_string(temperature_) +
		   "," + std::to_string(humidity_) + "," + std::to_string(weight_) + "\n";
}

boost::json::object ScaleMessage::getOutputProtocolEntry() const {
	boost::json::object obj {
			{ "timestamp_ms", currentTimestampMs_ },
			{ "flags",        flags_ },
			{ "temperature",  temperature_ },
			{ "humidity",     humidity_ },
			{ "weight",       weight_ }
	};
	return obj;
}


void ScaleMessage::setWeight(float weight) {
	weight_ = weight;
}

void ScaleMessage::setTemperature(float temperature) {
	temperature_ = temperature;
}

void ScaleMessage::setHumidity(float humidity) {
	humidity_ = humidity;
}

}
