#pragma once

#include <gateway/settings/Settings.hpp>

#include <memory>



namespace gateway::structures {
class GlobalContext {
public:
	[[nodiscard]] const std::shared_ptr<settings::Settings> &getSettings() const;

	void setSettings(const std::shared_ptr<settings::Settings> &settings);

private:
	std::shared_ptr<settings::Settings> settings_;
	//todo from boost

};
}
