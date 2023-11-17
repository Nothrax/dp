#include <gateway/structures/GLobalContext.hpp>



namespace gateway::structures {
const std::shared_ptr<settings::Settings> &GlobalContext::getSettings() const {
	return settings_;
}

void GlobalContext::setSettings(const std::shared_ptr<settings::Settings> &settings) {
	settings_ = settings;
}
}


