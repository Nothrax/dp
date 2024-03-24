#include <gateway/output_protocol/mqtt_protocol/Mqtt.hpp>
#include <gateway/common_tools/OutputProtocolTools.hpp>
#include <gateway/common_tools/TimeTools.hpp>
#include <gateway/logger/Logger.hpp>
#include <gateway/output_protocol/message/DataAck.hpp>
#include <gateway/output_protocol/message/DataRead.hpp>
#include <gateway/output_protocol/message/DataReadResponseAck.hpp>

#include <boost/json.hpp>



namespace gateway::output_protocol::mqtt_protocol {

Mqtt::Mqtt(const std::shared_ptr<structures::GlobalContext> &context): Output(context) {
	publishTopic_ = context->settings->getCompany() + "/" + context->settings->getGatewayId() + "/gateway";
	subscribeTopic_ = context->settings->getCompany() + "/" + context->settings->getGatewayId() + "/uploader";
	serverAddress_ =
			context->settings->getMqttBrokerAddress() + ":" + std::to_string(context->settings->getMqttBrokerPort());
	csvManager_ = std::make_unique<CsvManager>(context);
	csvManager_->initialize();
	messageAckTimer_ = std::make_unique<MessageAckTimer>(csvManager_, TIMEOUT);
	messageAckTimer_->startTimer();
	dataMessageAckTimer_ = std::make_unique<MessageAckTimer>(csvManager_, TIMEOUT);
	dataMessageAckTimer_->startTimer();
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

	auto ret = connect();

	if(!listenerThread_.joinable()) {
		listenerThread_ = std::thread(&Mqtt::listenerLoop, this);
	}

	return ret;
}

bool Mqtt::sendMessage(const std::shared_ptr<device::Entry> &message) {
	if(!client_ || !client_->is_connected()) {
		initialize();
		csvManager_->storeMessage(message);
		return false;
	}

	std::vector<std::shared_ptr<device::Entry>> messages;
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
	logger::Logger::logInfo("Published message with id: {}", dataId_);
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
		client_->subscribe(subscribeTopic_, QOS);
		logger::Logger::logInfo("Subscribed to topic {}", subscribeTopic_);
		logger::Logger::logInfo("Publishing to topic {}", publishTopic_);
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
		client_->unsubscribe(subscribeTopic_);
		client_->disconnect();
	}

	client_.reset();
}

Mqtt::~Mqtt() {
	if(listenerThread_.joinable()) {
		listenerThread_.join();
	}
}

void Mqtt::listenerLoop() {
	while(!context_->context.stopped()) {
		if(!client_ || !client_->is_connected()) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}

		auto message = client_->try_consume_message_for(std::chrono::seconds(5));
		if(message) {
			auto payload = message->to_string();
			try {
				handleUploaderMessage(payload);
			} catch(const std::exception &e) {
				logger::Logger::logError("Error handling uploader message: {}", e.what());
			} catch(...) {
				logger::Logger::logError("Error handling uploader message");
			}
		}
	}

}

void Mqtt::handleUploaderMessage(const std::string &message) {
	auto parsedMessage = boost::json::parse(message).as_object();

	auto messageType = common_tools::OutputProtocolTools::getMessageType(parsedMessage);

	switch(messageType) {
		case EMessageType::E_DATA_ACK:
			handleAckMessage(parsedMessage);
			break;
		case EMessageType::E_DATA_READ:
			handleDataReadMessage(parsedMessage);
			break;
		case EMessageType::E_DATA_READ_RESPONSE_ACK:
			handleDataReadResponseMessage(parsedMessage);
			break;
		default:
			logger::Logger::logError("Unsupported uploader message type");
	}
}

void Mqtt::handleAckMessage(const boost::json::object &message) {
	message::DataAck dataAck;
	dataAck.parse(message);
	if(dataAck.getResponseType() == EResponseType::E_OK) {
		messageAckTimer_->removeTimer(dataAck.getId());
		logger::Logger::logInfo("Ack message received for message id: {}", dataAck.getId());
	} else {
		logger::Logger::logError("Error handling ack message: {}", dataAck.getError());
	}
}

void Mqtt::handleDataReadMessage(const boost::json::object &message) {
	message::DataRead dataRead;
	dataRead.parse(message);
	auto messages = csvManager_->getStoredMessages(dataRead.getDeviceType(), dataRead.getDeviceNumber());

	if(!client_ || !client_->is_connected()) {
		initialize();
		return;
	}

	auto storedMessages = csvManager_->areStoredMessages(dataRead.getDeviceType(), dataRead.getDeviceNumber());
	auto dataReadResponseMessage = common_tools::OutputProtocolTools::generateDataReadResponseMessage(messages,
																									  dataRead.getId(),
																									  storedMessages,
																									  dataRead.getDeviceType(),
																									  dataRead.getDeviceNumber());
	const auto size = dataReadResponseMessage.size();
	if(dataReadResponseMessage.empty()) {
		logger::Logger::logError(
				"Cannot create DATA message, device number or type does not correspond to other messages.");
		csvManager_->storeMessages(messages);
		return;
	}

	client_->publish(publishTopic_, dataReadResponseMessage.c_str(), size, QOS, false);
	logger::Logger::logInfo("Published DATA_READ_RESPONSE message with id: {}", dataRead.getId());
	dataMessageAckTimer_->addTimer(messages, dataRead.getId());
}

void Mqtt::handleDataReadResponseMessage(const boost::json::object &message) {
	message::DataReadResponseAck dataAck;
	dataAck.parse(message);
	if(dataAck.getResponseType() == EResponseType::E_OK) {
		dataMessageAckTimer_->removeTimer(dataAck.getId());
		logger::Logger::logInfo("Data response ack message received for message id: {}", dataAck.getId());
	} else {
		logger::Logger::logError("Error handling data response ack message: {}", dataAck.getError());
	}
}

}
