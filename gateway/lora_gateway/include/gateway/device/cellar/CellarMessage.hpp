#pragma once

#include <gateway/device/Message.hpp>



namespace gateway::device::cellar {
class CellarMessage final: public Message {
public:
	explicit CellarMessage(uint32_t deviceNumber): Message(input_protocol::EDeviceType::E_WINE_CELLAR, deviceNumber) {};

	[[nodiscard]] std::string getCsvHeader() const override;

	[[nodiscard]] std::string getCsvEntry() const override;

	[[nodiscard]] boost::json::object getOutputProtocolEntry() const override;

	void setCo2(uint32_t co2);

	void setTemperature(float temperature);

	void setHumidity(float humidity);

private:
	uint32_t co2_ { 0 };
	float temperature_ { 0 };
	float humidity_ { 0 };
};
}
