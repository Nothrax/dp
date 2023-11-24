#include <gateway/common_tools/EnumTools.hpp>

#include <algorithm>



namespace gateway::common_tools {
settings::EDeviceType EnumTools::stringToDeviceType(std::string toEnum) {
	std::transform(toEnum.begin(), toEnum.end(), toEnum.begin(), ::toupper);
	if(toEnum == "LORA") {
		return settings::EDeviceType::E_LORA;
	}

	return gateway::settings::EDeviceType::E_INVALID;
}

settings::EOutputType EnumTools::stringToOutputType(std::string toEnum) {
	std::transform(toEnum.begin(), toEnum.end(), toEnum.begin(), ::toupper);
	if(toEnum == "CSV") {
		return settings::EOutputType::E_CSV;
	} else if(toEnum == "MQTT") {
		return settings::EOutputType::E_MQTT;
	}
	return gateway::settings::EOutputType::E_INVALID;
}
}


