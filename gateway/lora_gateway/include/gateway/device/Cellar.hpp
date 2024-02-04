#pragma once

#include <gateway/device/Device.hpp>

namespace gateway::device {
class Cellar final: public Device{
public:
	explicit Cellar(uint32_t deviceNumber): Device(structures::EDeviceType::E_WINE_CELLAR, deviceNumber) {};

	[[nodiscard]] std::string getCsvHeader() const override;

	[[nodiscard]] std::string getCsvEntry() const override;

	[[nodiscard]] boost::json::object getOutputProtocolEntry() const override;
private:
	uint32_t co2_{0};
	float temperature_{0};
	float humidity_{0};

	bool parseData(const structures::DeviceMessage &message) override;
};
}