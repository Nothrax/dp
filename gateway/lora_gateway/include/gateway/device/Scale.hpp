#pragma once

#include <gateway/device/Device.hpp>



namespace gateway::device {
class Scale: public Device {
public:
	explicit Scale(uint32_t deviceNumber): Device(structures::EDeviceType::E_BEE_SCALE, deviceNumber) {};
private:
};
}