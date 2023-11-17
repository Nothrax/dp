#include <gateway/Gateway.hpp>

namespace gateway{
uint8_t Gateway::calculateCheckSum(const uint8_t *data, size_t dataSize) {
	uint8_t sum = 0;
	for(int index = 0; index < dataSize; index++){
		sum += data[index];
	}
	return sum;
}
}