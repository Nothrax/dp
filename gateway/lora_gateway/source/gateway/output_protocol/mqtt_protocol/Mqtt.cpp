#include <gateway/output_protocol/mqtt_protocol/Mqtt.hpp>
#include <gateway/logger/Logger.hpp>



namespace gateway::output_protocol::mqtt_protocol {

Mqtt::Mqtt(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {
	publishTopic_ = context->settings->getCompany() + "/" + context->settings->getGatewayId();
	serverAddress_ =
			context->settings->getMqttBrokerAddress() + ":" + std::to_string(context->settings->getMqttBrokerPort());
}

bool Mqtt::initialize() {
	if(client_ && client_->is_connected()) {
		return true;
	}
	if(client_) {
		disconnect();
	}

	return connect();
}

bool Mqtt::writeMessage(const std::shared_ptr<device::Message> &message) {
	if(client_ == nullptr || not client_->is_connected()) {
		logger::Logger::logError("Mqtt client is not initialized or connected to the server");
		return false;
	}
	std::string payload = boost::json::serialize(message->getOutputProtocolEntry());
	const auto size = payload.size();

	client_->publish(publishTopic_, payload.c_str(), size, QOS, false);
	return true;
}

bool Mqtt::connect() {
	//todo ssl
	client_ = std::make_unique<mqtt::async_client>(serverAddress_, context_->settings->getMqttUsername(),
												   mqtt::create_options(MQTTVERSION_3_1_1));
	if(!client_) {
		return false;
	}

	mqtt::connect_options connopts_;
	if(context_->settings->isSslEnable()) {
		serverAddress_ = "ssl://" + serverAddress_;
		auto sslopts = mqtt::ssl_options_builder()
				.trust_store(context_->settings->getCaFile())
				.private_key(context_->settings->getClientKey())
				.key_store(context_->settings->getClientCertificate())
				.error_handler([](const std::string &msg) {
					logger::Logger::logError("MQTT SSL Error {}", msg);
				})
				.finalize();
		connopts_.set_ssl(sslopts);
	}

	client_->start_consuming();
	mqtt::token_ptr conntok = client_->connect(connopts_);
	conntok->wait();
	logger::Logger::logInfo("Connected to MQTT server {}", serverAddress_);

	return client_->is_connected();
}

void Mqtt::disconnect() {
	if(!client_) {
		return;
	}
	if(client_->is_connected()) {
		client_->disconnect();
	}
	client_.reset();
}

}
