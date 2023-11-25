#include <gateway/endpoint/generator/GeneratorEndpoint.hpp>



namespace gateway::endpoint::generator {

bool GeneratorEndpoint::initialize() {
	initialized_ = true;
	return initialized_;
}

std::shared_ptr<structures::DeviceMessage> GeneratorEndpoint::getMessage(unsigned int timeoutMs) const {
	//todo: implement
	return nullptr;
}
}