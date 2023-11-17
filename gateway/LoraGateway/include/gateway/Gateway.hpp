#pragma once

namespace gateway{
class Gateway {
public:
private:


	uint8_t calculateCheckSum(const uint8_t *data, size_t dataSize);
};
}