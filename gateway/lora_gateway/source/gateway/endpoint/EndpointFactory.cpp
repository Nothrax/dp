#include <gateway/endpoint/EndpointFactory.hpp>
#include <gateway/endpoint/lora/LoraEndpoint.hpp>
#include <gateway/endpoint/generator/GeneratorEndpoint.hpp>



namespace gateway::endpoint {

std::shared_ptr<Endpoint> EndpointFactory::createEndpoint(const std::shared_ptr<structures::GlobalContext> &context) {
	switch(context->settings->getDeviceType()) {
		case settings::EDeviceType::E_LORA:
			return std::make_shared<lora::LoraEndpoint>(context);
		case settings::EDeviceType::E_GENERATOR:
			return std::make_shared<generator::GeneratorEndpoint>(context);
		default:
			return nullptr;
	}
}
}