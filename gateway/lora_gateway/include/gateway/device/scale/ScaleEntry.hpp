#pragma once

#include <gateway/device/Entry.hpp>



namespace gateway::device::scale {
class ScaleEntry final: public Entry {
public:
	explicit ScaleEntry(uint32_t deviceNumber): Entry(input_protocol::EDeviceType::E_BEE_SCALE, deviceNumber) {};

	[[nodiscard]] static std::string getCsvHeader();

	[[nodiscard]] std::string getCsvEntry() const override;

	[[nodiscard]] boost::json::object getOutputProtocolEntry() const override;

	void setFromCsvEntry(const std::string &csvEntry) override;

	void setWeight(float weight);

	void setTemperature(float temperature);

	void setHumidity(float humidity);

private:
	float weight_ { 0 };
	float temperature_ { 0 };
	float humidity_ { 0 };
};
}
