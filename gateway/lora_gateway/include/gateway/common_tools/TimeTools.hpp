#pragma once

#include <cstdint>



namespace gateway::common_tools {
class TimeTools {
public:
	/**
	 * @brief Return a unix UTC timestamp in millisecond precision
	 * @return timestamp
	 */
	static uint64_t getUnixTimestampMs();

	/**
	 * @brief Return a unix UTC timestamp in second precision
	 * @return timestamp
	 */
	static uint64_t getUnixTimestampS();

};
}