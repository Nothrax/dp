#pragma once

#include <gateway/device/Device.hpp>

namespace gateway::device {
class Cellar: public Device{
public:
	explicit Cellar(uint32_t deviceNumber): Device(structures::EDeviceType::E_WINE_CELLAR, deviceNumber) {};
private:
};
}