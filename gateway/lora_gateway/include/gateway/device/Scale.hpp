#pragma once

#include <gateway/device/Device.hpp>



namespace gateway::device {
class Scale final: public Device {
public:
	explicit Scale(uint32_t deviceNumber): Device(structures::EDeviceType::E_BEE_SCALE, deviceNumber) {};

	[[nodiscard]] std::string getCsvHeader() const override;

	[[nodiscard]] std::string getCsvEntry() const override;

	[[nodiscard]] boost::json::object getOutputProtocolEntry() const override;
private:
	float weight_{0};
	float temperature_{0};
	float humidity_{0};

	bool parseData(const structures::DeviceMessage &message) override;
};
}