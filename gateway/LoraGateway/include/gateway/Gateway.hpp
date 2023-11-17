#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/device_communication/LoraEndpoint.hpp>

#include <cstdint>

namespace gateway{
class Gateway {
public:
	explicit Gateway(const std::shared_ptr<structures::GlobalContext> &globalContext): context_(globalContext){};

	void start();

private:
	std::shared_ptr<structures::GlobalContext> context_;
	std::unique_ptr<device_communication::LoraEndpoint> loraEndpoint_;

	uint8_t calculateCheckSum(const uint8_t *data, size_t dataSize);
};
}