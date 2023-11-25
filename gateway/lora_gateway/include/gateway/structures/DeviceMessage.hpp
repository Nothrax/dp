#pragma once

#include <cstdint>



namespace gateway::structures {

enum class EDeviceType {
	E_BEGIN = -1,
	E_INVALID = E_BEGIN,
	E_WINE_CELLAR,
	E_BEE_SCALE,
	E_END
};

#pragma pack(push, 1)
/// v2
struct DeviceMessage {
	uint8_t protocolVersion { 0 };
	uint8_t deviceType { 0 };
	uint8_t deviceNumber { 0 };
	uint8_t flags { 0 };
	uint32_t values[5] { 0 };
	uint8_t checkSum { 0 };
};
#pragma pack(pop)
}



