#pragma once

#include <gateway/device/Message.hpp>



namespace gateway::device::scale {
class ScaleMessage final: public Message {
public:
	explicit ScaleMessage(uint32_t deviceNumber): Message(input_protocol::EDeviceType::E_BEE_SCALE, deviceNumber) {};

	[[nodiscard]] std::string getCsvHeader() const override;

	[[nodiscard]] std::string getCsvEntry() const override;

	[[nodiscard]] boost::json::object getOutputProtocolEntry() const override;

	void setWeight(float weight);

	void setTemperature(float temperature);

	void setHumidity(float humidity);

private:
	float weight_ { 0 };
	float temperature_ { 0 };
	float humidity_ { 0 };
};
}
