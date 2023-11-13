//
// Created by trubk on 19/08/2020.
//

#ifndef LORAGATEWAY_ARGUMENTS_H
#define LORAGATEWAY_ARGUMENTS_H

#include <string>
#include <iostream>
#include <getopt.h>

class Arguments {
public:
    static bool getVerboseFromArgument(int argc, char **argv);
};


#endif //LORAGATEWAY_ARGUMENTS_H
