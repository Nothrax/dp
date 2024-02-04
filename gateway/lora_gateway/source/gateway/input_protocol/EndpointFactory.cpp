#include <gateway/input_protocol/EndpointFactory.hpp>
#include <gateway/input_protocol/lora/LoraEndpoint.hpp>
#include <gateway/input_protocol/generator/GeneratorEndpoint.hpp>



namespace gateway::input_protocol {

std::shared_ptr<Endpoint> EndpointFactory::createEndpoint(const std::shared_ptr<structures::GlobalContext> &context) {
	switch(context->settings->getDeviceType()) {
		case settings::EDeviceCommunicationType::E_LORA:
			return std::make_shared<lora::LoraEndpoint>(context);
		case settings::EDeviceCommunicationType::E_GENERATOR:
			return std::make_shared<generator::GeneratorEndpoint>(context);
		default:
			return nullptr;
	}
}
}