#include <gateway/output_protocol/message/DataReadResponseAck.hpp>

namespace gateway::output_protocol::message {

void DataReadResponseAck::parse(const boost::json::object &message) {
	id_ = message.at("id").as_int64();
	responseType_ = static_cast<EResponseType>(message.at("response_type").as_int64());
	error_ = message.at("error_data").as_string().c_str();
}

EResponseType DataReadResponseAck::getResponseType() const {
	return responseType_;
}

void DataReadResponseAck::setResponseType(EResponseType responseType) {
	responseType_ = responseType;
}

const std::string &DataReadResponseAck::getError() const {
	return error_;
}

void DataReadResponseAck::setError(const std::string &error) {
	error_ = error;
}
}