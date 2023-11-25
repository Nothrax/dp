#pragma once

#include <gateway/output/Output.hpp>



namespace gateway::output::mqtt {
class Mqtt final: public Output {
public:
	explicit Mqtt(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {};

	bool initialize() override;

	bool writeFromDevice(const std::shared_ptr<device::Device> &device) override;

	~Mqtt() override = default;

private:
};
}