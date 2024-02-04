#include <gateway/device/DeviceFactory.hpp>
#include <gateway/device/scale/ScaleDevice.hpp>
#include <gateway/device/cellar/CellarDevice.hpp>
#include <gateway/common_tools/EnumTools.hpp>

namespace gateway::device {
std::shared_ptr<Device> DeviceFactory::createDevice(uint32_t deviceType, uint32_t deviceNumber) {
	auto deviceEnum = common_tools::EnumTools::valueToEnum<input_protocol::EDeviceType>(deviceType);
	switch(deviceEnum) {
		case input_protocol::EDeviceType::E_BEE_SCALE:
			return std::make_shared<scale::ScaleDevice>(deviceNumber);
		case input_protocol::EDeviceType::E_WINE_CELLAR:
			return std::make_shared<cellar::CellarDevice>(deviceNumber);
		default:
			return nullptr;
	}
}
}