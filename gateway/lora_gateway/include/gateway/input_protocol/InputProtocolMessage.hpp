#pragma once

#include <cstdint>



namespace gateway::input_protocol{

/**
 * @brief Enum representing device types
 */
enum class EDeviceType {
	E_WINE_CELLAR = 1,
	E_BEE_SCALE = 2
};

#pragma pack(push, 1)
/**
 * @brief Struct representing transmitted message from device
 */
struct InputProtocolMessage {
	uint8_t protocolVersion { 0 };
	uint8_t deviceType { 0 };
	uint8_t deviceNumber { 0 };
	uint8_t flags { 0 };
	uint32_t values[5] { 0 };
	uint8_t checkSum { 0 };
};
#pragma pack(pop)
}



