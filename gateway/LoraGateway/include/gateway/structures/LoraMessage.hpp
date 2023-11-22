#pragma once

#include <cstdint>



namespace gateway::structures {
#pragma pack(push, 1)
struct LoraMessage {
	uint8_t protocolVersion = 0;
	uint16_t unitNumber = 0;
	uint8_t flags = 0;
	uint32_t values[5] = { 0 };
	uint8_t checkSum = 0;
};
#pragma pack(pop)
}



