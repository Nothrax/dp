#include <gateway/device/DeviceFactory.hpp>
#include <gateway/device/Scale.hpp>
#include <gateway/device/Cellar.hpp>
#include <gateway/common_tools/EnumTools.hpp>

namespace gateway::device {
std::shared_ptr<Device> DeviceFactory::createDevice(uint32_t deviceType, uint32_t deviceNumber) {
	auto deviceEnum = common_tools::EnumTools::valueToEnum<structures::EDeviceType>(deviceType);
	switch(deviceEnum) {
		case structures::EDeviceType::E_BEE_SCALE:
			return std::make_shared<Scale>(deviceNumber);
		case structures::EDeviceType::E_WINE_CELLAR:
			return std::make_shared<Cellar>(deviceNumber);
		default:
			return nullptr;
	}
}
}