#include <gateway/common_tools/TimeTools.hpp>

#include <chrono>



namespace gateway::common_tools {

namespace sc = std::chrono;

uint64_t TimeTools::getUnixTimestampMs() {
	const auto timestamp = sc::system_clock::now();
	auto since_epoch = timestamp.time_since_epoch();
	auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
	return millis.count();
}

uint64_t TimeTools::getUnixTimestampS() {
	const auto timestamp = sc::system_clock::now();
	auto since_epoch = timestamp.time_since_epoch();
	auto seconds = sc::duration_cast<sc::seconds>(since_epoch);
	return seconds.count();
}
}
