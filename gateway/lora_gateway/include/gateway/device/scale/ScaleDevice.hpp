#pragma once

#include <gateway/device/Device.hpp>

namespace gateway::device::scale {
class ScaleDevice final: public Device{
public:
	explicit ScaleDevice(uint32_t deviceNumber): Device(input_protocol::EDeviceType::E_BEE_SCALE, deviceNumber) {};

	std::shared_ptr<Message> parseMessage(const input_protocol::InputProtocolMessage &message) override;

private:
	bool parseDeviceSpecificFlags(const uint8_t &flags) override;
};
}