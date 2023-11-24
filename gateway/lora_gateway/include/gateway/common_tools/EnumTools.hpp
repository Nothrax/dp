#pragma once

#include <gateway/settings/Settings.hpp>


/**
 * @brief Class for work with enum
 */
namespace gateway::common_tools {
class EnumTools {
public:
	/**
	 * @brief Translate string to EDeviceType
	 * @param deviceType string to be translated
	 * @return translated enum value, E_INVALID in case of an error
	 */
	static settings::EDeviceType stringToDeviceType(std::string toEnum);

	/**
	 * @brief Translate string to EOutputType
	 * @param deviceType string to be translated
	 * @return translated enum value, E_INVALID in case of an error
	 */
	static settings::EOutputType stringToOutputType(std::string toEnum);

};
}
