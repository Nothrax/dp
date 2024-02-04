#pragma once

#include <gateway/device/Device.hpp>

namespace gateway::device::cellar {
class CellarDevice final: public Device{
public:
	explicit CellarDevice(uint32_t deviceNumber): Device(input_protocol::EDeviceType::E_WINE_CELLAR, deviceNumber) {};

	std::shared_ptr<Message> parseMessage(const input_protocol::InputProtocolMessage &message) override;

private:
	bool parseDeviceSpecificFlags(const uint8_t &flags) override;
};
}