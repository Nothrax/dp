#pragma once

#include <gateway/device/Entry.hpp>



namespace gateway::device::cellar {
class CellarEntry final: public Entry {
public:
	explicit CellarEntry(uint32_t deviceNumber): Entry(input_protocol::EDeviceType::E_WINE_CELLAR, deviceNumber) {};

	[[nodiscard]] static std::string getCsvHeader();

	[[nodiscard]] std::string getCsvEntry() const override;

	[[nodiscard]] boost::json::object getOutputProtocolEntry() const override;

	void setFromCsvEntry(const std::string &csvEntry) override;

	void setCo2(uint32_t co2);

	void setTemperature(float temperature);

	void setHumidity(float humidity);

private:
	uint32_t co2_ { 0 };
	float temperature_ { 0 };
	float humidity_ { 0 };
};
}
