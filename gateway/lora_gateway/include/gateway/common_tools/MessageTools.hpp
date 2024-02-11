#pragma once

#include <cstdint>



namespace gateway::common_tools {
class MessageTools {
public:
	/**
	 * @brief Calculate 1B long checksum of given data
	 * @param data pointer to data
	 * @param length length of data
	 * @return checksum
	 */
	static uint8_t calculateChecksum(const uint8_t *data, uint8_t length);
};
}