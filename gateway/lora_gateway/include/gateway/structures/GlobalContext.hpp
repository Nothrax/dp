#pragma once

#include <gateway/settings/Settings.hpp>

#include <boost/asio.hpp>

#include <memory>



namespace gateway::structures {
/**
 * @brief Global context of the gateway
 */
class GlobalContext {
public:
	/// Settings of the gateway
	std::shared_ptr<settings::Settings> settings;
	/// Boost context
	boost::asio::io_context context;
};
}
