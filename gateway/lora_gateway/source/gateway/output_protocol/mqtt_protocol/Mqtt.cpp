#include <gateway/output_protocol/mqtt_protocol/Mqtt.hpp>
#include <gateway/common_tools/OutputProtocolTools.hpp>
#include <gateway/common_tools/TimeTools.hpp>
#include <gateway/logger/Logger.hpp>



namespace gateway::output_protocol::mqtt_protocol {

//todo subscribe thread for deleting acked messages

Mqtt::Mqtt(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {
	publishTopic_ = context->settings->getCompany() + "/" + context->settings->getGatewayId();
	serverAddress_ =
			context->settings->getMqttBrokerAddress() + ":" + std::to_string(context->settings->getMqttBrokerPort());
	csvManager_ = std::make_unique<CsvManager>(context);
	csvManager_->initialize();
	messageAckTimer_ = std::make_unique<MessageAckTimer>(csvManager_, TIMEOUT);
	messageAckTimer_->startTimer();
}

bool Mqtt::initialize() {
	auto currentTime = common_tools::TimeTools::getUnixTimestampS();
	if(currentTime - lastReconnectAttempt_ < RECONNECT_PERIOD) {
		return false;
	}
	lastReconnectAttempt_ = currentTime;
	logger::Logger::logInfo("Initializing MQTT connection");
	if(client_ && client_->is_connected()) {
		return true;
	}
	if(client_) {
		disconnect();
	}

	return connect();
}

bool Mqtt::sendMessage(const std::shared_ptr<device::Message> &message) {
	if(client_ == nullptr || not client_->is_connected()) {
		initialize();
		csvManager_->storeMessage(message);
		return false;
	}

	std::vector<std::shared_ptr<device::Message>> messages;
	messages.push_back(message);
	auto storedMessages = csvManager_->areStoredMessages(message->getDeviceType(), message->getDeviceNumber());
	auto dataMessage = common_tools::OutputProtocolTools::generateDataMessage(messages, dataId_, storedMessages,
																			  message->getDeviceType(),
																			  message->getDeviceNumber());
	const auto size = dataMessage.size();
	if(dataMessage.empty()) {
		logger::Logger::logError(
				"Cannot create DATA message, device number or type does not correspond to other messages.");
		csvManager_->storeMessage(message);
		return false;
	}

	client_->publish(publishTopic_, dataMessage.c_str(), size, QOS, false);
	messageAckTimer_->addTimer(messages, dataId_);
	dataId_++;
	return true;
}

bool Mqtt::connect() {
	//todo ssl
	client_ = std::make_unique<mqtt::async_client>(serverAddress_, context_->settings->getMqttUsername(),
												   mqtt::create_options(MQTTVERSION_3_1_1));
	if(!client_) {
		return false;
	}

	try {
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
	} catch(const std::exception &e) {
		logger::Logger::logError("Failed to connect to MQTT server: {}", e.what());
		return false;
	}


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
