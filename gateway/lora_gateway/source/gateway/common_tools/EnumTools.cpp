#include <gateway/common_tools/EnumTools.hpp>

#include <algorithm>



namespace gateway::common_tools {

template <>
settings::EOutputType EnumTools::stringToEnum(std::string toEnum) {
	std::transform(toEnum.begin(), toEnum.end(), toEnum.begin(), ::toupper);
	if(toEnum == "CSV") {
		return settings::EOutputType::E_CSV;
	} else if(toEnum == "MQTT") {
		return settings::EOutputType::E_MQTT;
	}
	return gateway::settings::EOutputType::E_INVALID;
}

template <>
settings::EDeviceType EnumTools::stringToEnum(std::string toEnum) {
	std::transform(toEnum.begin(), toEnum.end(), toEnum.begin(), ::toupper);
	if(toEnum == "LORA") {
		return settings::EDeviceType::E_LORA;
	} else if(toEnum == "GENERATOR") {
		return settings::EDeviceType::E_GENERATOR;
	}

	return gateway::settings::EDeviceType::E_INVALID;
}

template <>
std::string EnumTools::enumToString(settings::EOutputType toEnum) {
	switch(toEnum) {
		case settings::EOutputType::E_CSV:
			return "CSV";
		case settings::EOutputType::E_MQTT:
			return "MQTT";
		default:
			return "INVALID";
	}
}

template <>
std::string EnumTools::enumToString(settings::EDeviceType toEnum) {
	switch(toEnum) {
		case settings::EDeviceType::E_LORA:
			return "LORA";
		case settings::EDeviceType::E_GENERATOR:
			return "GENERATOR";
		default:
			return "INVALID";
	}
}
}


