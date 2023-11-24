#pragma once

#include <gateway/settings/Settings.hpp>

#include <memory>

namespace gateway::settings{
class SettingsParser {
public:
	/**
	 * @brief Parse settings from command line arguments and config file
	 * @param argc count of arguments
	 * @param argv arguments
	 * @return true if settings are valid
	 * @return false if settings are not valid
	 */
	bool parseSettings(int argc, char **argv);

	[[nodiscard]] const std::shared_ptr<Settings> &getSettings() const;
private:
	std::shared_ptr<Settings> settings_;
	std::string configPath_;

	bool parseCmdArguments(int argc, char **argv);

	bool parseConfig();

	bool isSettingValid();

};
}