#include <gateway/output/OutputFactory.hpp>
#include <gateway/output/csv/CsvOutput.hpp>
#include <gateway/output/mqtt_protocol/Mqtt.hpp>



namespace gateway::output {
std::shared_ptr<Output>
gateway::output::OutputFactory::createOutput(const std::shared_ptr<structures::GlobalContext> &context) {
	switch(context->settings->getOutputType()) {
		case settings::EOutputType::E_CSV:
			return std::make_shared<csv::CsvOutput>(context);
		case settings::EOutputType::E_MQTT:
			return std::make_shared<mqtt_protocol::Mqtt>(context);
		default:
			return nullptr;
	}
}
}


