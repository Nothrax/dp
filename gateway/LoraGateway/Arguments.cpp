//
// Created by trubk on 19/08/2020.
//

#include "Arguments.h"
bool Arguments::getVerboseFromArgument(int argc, char **argv){
    bool verbose = false;
    int c;
    while ((c = getopt(argc, argv, "v")) != -1)
        if (c == 'v') {
            verbose = true;
        }
    return verbose;
}