#include <gateway/output_protocol/MessageAckTimer.hpp>
#include <gateway/common_tools/TimeTools.hpp>
#include <gateway/logger/Logger.hpp>

#include <iostream>



namespace gateway::output_protocol {

void MessageAckTimer::startTimer() {
	timerThread_ = std::thread(&MessageAckTimer::timerLoop, this);
}

void MessageAckTimer::addTimer(const std::vector<std::shared_ptr<device::Message>> &messages, uint32_t dataId) {
	std::scoped_lock lock(mutex_);
	TimerData timerData;
	timerData.messages = messages;
	timerData.dataId = dataId;
	timerData.startTime = common_tools::TimeTools::getUnixTimestampS();
	timers_[dataId] = timerData;
}

void MessageAckTimer::removeTimer(uint32_t dataId) {
	std::scoped_lock lock(mutex_);
	std::cout << "Removing timer for dataId " << dataId << std::endl;
	timers_.erase(dataId);
}

MessageAckTimer::~MessageAckTimer() {
	running_ = false;
	timerThread_.join();
}

void MessageAckTimer::timerLoop() {
	while(running_) {
		tick();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void MessageAckTimer::tick() {
	std::scoped_lock lock(mutex_);
	auto currentTime = common_tools::TimeTools::getUnixTimestampS();
	for(auto it = timers_.begin(); it != timers_.end();) {
		if(currentTime - it->second.startTime > timeout_) {
			logger::Logger::logError("Timeout for dataId {}, messages will be stored in csv file.", it->first);
			csvManager_->storeMessages(it->second.messages);
			it = timers_.erase(it);
		} else {
			++it;
		}
	}
}
}
