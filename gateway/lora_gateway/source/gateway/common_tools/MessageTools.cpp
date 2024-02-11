#include <gateway/common_tools/MessageTools.hpp>



namespace gateway::common_tools {
uint8_t MessageTools::calculateChecksum(const uint8_t *data, uint8_t length) {
	uint8_t sum = 0;
	for(int index = 0; index < length; index++) {
		sum += data[index];
	}
	return sum;
}
}
