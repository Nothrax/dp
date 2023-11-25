#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/endpoint/Endpoint.hpp>
#include <gateway/output/csv/CsvOutput.hpp>

#include <cstdint>



namespace gateway {
class Gateway {
public:
	explicit Gateway(const std::shared_ptr<structures::GlobalContext> &globalContext): context_(globalContext) {};

	void start();

private:
	std::shared_ptr<structures::GlobalContext> context_;
	std::shared_ptr<endpoint::Endpoint> endpoint;
	std::shared_ptr<output::csv::CsvOutput> csvWriter_;
	uint8_t messageCounter_ { 0 };
	static constexpr uint32_t messageReceiveTimeoutMs_ { 5000 };

	bool initialize();

	void processMessage(const structures::DeviceMessage &message);
};
}