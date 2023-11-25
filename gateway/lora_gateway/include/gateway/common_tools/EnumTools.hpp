#pragma once

#include <string>



namespace gateway::common_tools {
/**
 * @brief Class for work with enum
 */
class EnumTools {
public:
	/**
	 * @brief Translate string to given enum type
	 * @param toEnum value to be translated
	 * @return translated enum value, E_INVALID in case of an error
	 */
	template <typename T, typename L>
	static T valueToEnum(L toEnum);

	/**
	 * @brief Translate enum to string representation
	 * @tparam T enum type
	 * @param toString enum value to be translated
	 * @return value in case of success, empty if not successful
	 */
	template <typename T>
	static std::string enumToString(T toString);
};
}
