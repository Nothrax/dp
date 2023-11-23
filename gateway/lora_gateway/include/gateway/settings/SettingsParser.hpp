#pragma once

#include <gateway/settings/Settings.hpp>

#include <memory>

namespace gateway::settings{
class SettingsParser {
public:
	bool parseSettings(int argc, char **argv);

	[[nodiscard]] const std::shared_ptr<Settings> &getSettings() const;
private:
	std::shared_ptr<Settings> settings_;

};
}