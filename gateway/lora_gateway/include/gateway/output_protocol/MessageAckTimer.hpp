#pragma once

#include <gateway/device/Message.hpp>
#include <gateway/output_protocol/CsvManager.hpp>

#include <map>
#include <memory>
#include <cstdint>
#include <thread>



namespace gateway::output_protocol {
class MessageAckTimer {
private:
	struct TimerData {
		std::vector<std::shared_ptr<device::Message>> messages;
		uint32_t dataId;
		uint32_t startTime;
	};
public:
	MessageAckTimer(const std::shared_ptr<CsvManager> &csvManager, uint32_t timeout): csvManager_(csvManager),
																					  timeout_(timeout) {};

	void startTimer();

	void addTimer(const std::vector<std::shared_ptr<device::Message>> &messages, uint32_t dataId);

	void removeTimer(uint32_t dataId);

	~MessageAckTimer();

private:
	std::shared_ptr<CsvManager> csvManager_;
	std::thread timerThread_;
	uint32_t timeout_;
	bool running_ { true };
	std::map<uint32_t, TimerData> timers_;
	std::mutex mutex_;

	void timerLoop();

	void tick();
};
}