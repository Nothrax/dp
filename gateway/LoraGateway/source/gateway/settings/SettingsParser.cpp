
#include <gateway/settings/SettingsParser.hpp>

namespace gateway::settings{
bool SettingsParser::parseSettings(int argc, char **argv){
    bool verbose = false;
    int c;
    while ((c = getopt(argc, argv, "v")) != -1)
        if (c == 'v') {
            verbose = true;
        }
    return verbose;
}

const std::shared_ptr<Settings> &SettingsParser::getSettings() const {
	return settings_;
}
}