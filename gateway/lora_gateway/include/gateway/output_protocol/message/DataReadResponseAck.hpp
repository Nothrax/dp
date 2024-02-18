#pragma once


#include <gateway/common_tools/OutputProtocolTools.hpp>
#include <gateway/output_protocol/OutputProtocolMessage.hpp>

namespace gateway::output_protocol::message {
class DataReadResponseAck: public OutputProtocolMessage {
public:
	explicit DataReadResponseAck(): OutputProtocolMessage(EMessageType::E_DATA_READ) {};

	void parse(const boost::json::object &message);

	[[nodiscard]] EResponseType getResponseType() const;

	void setResponseType(EResponseType responseType);

	[[nodiscard]] const std::string &getError() const;

	void setError(const std::string &error);

private:
	EResponseType responseType_ { EResponseType::E_INVALID };
	std::string error_;
};
}