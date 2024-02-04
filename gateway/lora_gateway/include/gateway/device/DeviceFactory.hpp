#pragma once

#include <gateway/device/Device.hpp>

#include <memory>



namespace gateway::device {
class DeviceFactory {
public:
	static std::shared_ptr<Device> createDevice(uint32_t deviceType, uint32_t deviceNumber);
};
}
