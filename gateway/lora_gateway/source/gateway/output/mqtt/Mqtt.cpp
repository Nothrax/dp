#include <gateway/output/mqtt/Mqtt.hpp>
#include <gateway/logger/Logger.hpp>



namespace gateway::output::mqtt {

bool Mqtt::initialize() {
	logger::Logger::logError("Mqtt output is not implemented");
	return false;
}

bool Mqtt::writeFromDevice(const std::shared_ptr<device::Device> &device) {
	logger::Logger::logError("Mqtt output is not implemented");
	return false;
}
}
