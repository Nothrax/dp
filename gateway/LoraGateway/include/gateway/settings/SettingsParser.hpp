#pragma once

#include <string>
#include <iostream>
#include <getopt.h>

namespace gateway::settings{
class Arguments {
public:
	//todo from json
    static bool getVerboseFromArgument(int argc, char **argv);
private:

};
}