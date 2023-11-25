#pragma once

#include <gateway/settings/Settings.hpp>

#include <boost/asio.hpp>

#include <memory>



namespace gateway::structures {
class GlobalContext {
public:
	std::shared_ptr<settings::Settings> settings;
	/// Boost context
	boost::asio::io_context context;
};
}
