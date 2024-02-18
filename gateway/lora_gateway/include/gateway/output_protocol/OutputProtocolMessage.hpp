#pragma once

#include <cstdint>



namespace gateway::output_protocol {
//todo static check if the enum is the same as in the input protocol
enum class EMessageType {
	E_INVALID = -1,
	E_DATA = 1,
	E_DATA_ACK = 2,
	E_DATA_READ = 3,
	E_DATA_READ_RESPONSE = 4,
	E_DATA_READ_RESPONSE_ACK = 5,
};

enum class EResponseType {
	E_INVALID = -1,
	E_OK = 0,
	E_ERROR = 1,
};

class OutputProtocolMessage {
public:
	explicit OutputProtocolMessage(EMessageType messageType): messageType_(messageType) {}

	[[nodiscard]] EMessageType getMessageType() const;

	[[nodiscard]] uint32_t getId() const;

	void setId(uint32_t id);

protected:
	static_assert(static_cast<int>(EMessageType::E_DATA) == 1, "EMessageType::E_DATA is not 1");
	static_assert(static_cast<int>(EMessageType::E_DATA_ACK) == 2, "EMessageType::E_DATA_ACK is not 2");
	static_assert(static_cast<int>(EMessageType::E_DATA_READ) == 3, "EMessageType::E_DATA_READ is not 3");
	static_assert(static_cast<int>(EMessageType::E_DATA_READ_RESPONSE) == 4, "EMessageType::E_DATA_READ_RESPONSE is not 4");
	static_assert(static_cast<int>(EMessageType::E_DATA_READ_RESPONSE_ACK) == 5, "EMessageType::E_DATA_READ_RESPONSE_ACK is not 5");
	EMessageType messageType_ { EMessageType::E_INVALID };
	uint32_t id_ { 0 };
};

}