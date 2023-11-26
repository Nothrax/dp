#pragma once

#include <gateway/structures/GlobalContext.hpp>
#include <gateway/endpoint/Endpoint.hpp>
#include <gateway/output/Output.hpp>
#include <gateway/device/Device.hpp>

#include <cstdint>
#include <map>



namespace gateway {
class Gateway {
public:
	explicit Gateway(const std::shared_ptr<structures::GlobalContext> &globalContext): context_(globalContext) {};

	void start();

private:
	std::shared_ptr<structures::GlobalContext> context_;
	std::shared_ptr<endpoint::Endpoint> endpoint_;
	std::shared_ptr<output::Output> output_;
	std::map<uint32_t, std::map<uint32_t, std::shared_ptr<device::Device>>> devices_;
	static constexpr uint32_t messageReceiveTimeoutMs_ { 2000 };

	bool initialize();

	void processMessage(const structures::DeviceMessage &message);
};
}