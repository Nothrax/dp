#include <gateway/output_protocol/OutputFactory.hpp>
#include <gateway/output_protocol/csv/CsvOutput.hpp>
#include <gateway/output_protocol/mqtt_protocol/Mqtt.hpp>



namespace gateway::output_protocol {
std::shared_ptr<Output> OutputFactory::createOutput(const std::shared_ptr<structures::GlobalContext> &context) {
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


