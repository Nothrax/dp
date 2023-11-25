#include <gateway/common_tools/EnumTools.hpp>
#include <gateway/settings/Settings.hpp>
#include <gateway/structures/DeviceMessage.hpp>

#include <algorithm>



namespace gateway::common_tools {

template <>
settings::EOutputType EnumTools::valueToEnum(std::string toEnum) {
	std::transform(toEnum.begin(), toEnum.end(), toEnum.begin(), ::toupper);
	if(toEnum == "CSV") {
		return settings::EOutputType::E_CSV;
	} else if(toEnum == "MQTT") {
		return settings::EOutputType::E_MQTT;
	}
	return gateway::settings::EOutputType::E_INVALID;
}

template <>
settings::EDeviceCommunicationType EnumTools::valueToEnum(std::string toEnum) {
	std::transform(toEnum.begin(), toEnum.end(), toEnum.begin(), ::toupper);
	if(toEnum == "LORA") {
		return settings::EDeviceCommunicationType::E_LORA;
	} else if(toEnum == "GENERATOR") {
		return settings::EDeviceCommunicationType::E_GENERATOR;
	}

	return gateway::settings::EDeviceCommunicationType::E_INVALID;
}

template <>
structures::EDeviceType EnumTools::valueToEnum(uint32_t toEnum) {
	switch(toEnum) {
		case 1:
			return structures::EDeviceType::E_WINE_CELLAR;
		case 2:
			return structures::EDeviceType::E_BEE_SCALE;
		default:
			return structures::EDeviceType::E_INVALID;
	}
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
std::string EnumTools::enumToString(settings::EDeviceCommunicationType toEnum) {
	switch(toEnum) {
		case settings::EDeviceCommunicationType::E_LORA:
			return "LORA";
		case settings::EDeviceCommunicationType::E_GENERATOR:
			return "GENERATOR";
		default:
			return "INVALID";
	}
}

template <>
std::string EnumTools::enumToString(structures::EDeviceType toEnum) {
	switch(toEnum) {
		case structures::EDeviceType::E_WINE_CELLAR:
			return "WINE_CELLAR";
		case structures::EDeviceType::E_BEE_SCALE:
			return "BEE_SCALE";
		default:
			return "INVALID";
	}
}
}


