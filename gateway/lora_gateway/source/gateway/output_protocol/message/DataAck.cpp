#include <gateway/output_protocol/message/DataAck.hpp>
#include <gateway/common_tools/EnumTools.hpp>

namespace gateway::output_protocol::message {

void DataAck::parse(const boost::json::object &message) {
	id_ = message.at("id").as_int64();
	responseType_ = static_cast<EResponseType>(message.at("response_type").as_int64());
	error_ = message.at("error_data").as_string().c_str();
}

EResponseType DataAck::getResponseType() const {
	return responseType_;
}

void DataAck::setResponseType(EResponseType responseType) {
	responseType_ = responseType;
}

const std::string &DataAck::getError() const {
	return error_;
}

void DataAck::setError(const std::string &error) {
	error_ = error;
}
}