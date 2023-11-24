#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/device/LoraEndpoint.hpp>
#include <gateway/cloud/CsvWriter.hpp>

#include <cstdint>



namespace gateway {
class Gateway {
public:
	explicit Gateway(const std::shared_ptr<structures::GlobalContext> &globalContext): context_(globalContext) {};

	void start();

private:
	std::shared_ptr<structures::GlobalContext> context_;
	std::unique_ptr<device::LoraEndpoint> loraEndpoint_;
	std::unique_ptr<cloud::CsvWriter> csvWriter_;
	uint8_t messageCounter_ { 0 };

	uint8_t calculateCheckSum(const uint8_t *data, size_t dataSize);

	bool initialize();

	void processMessage(const structures::DeviceMessage &message);
};
}