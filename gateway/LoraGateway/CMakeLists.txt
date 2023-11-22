CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
SET(CMAKE_CXX_STANDARD 20)

PROJECT(LoraGateway)

SET(LoraGateway_VERSION 1.0.0)

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(Boost 1.79.0 REQUIRED COMPONENTS program_options json)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})


FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(lora_gateway_lib STATIC ${source_files}
        include/wiringPi.h
        include/wiringSerial.h
        source/gateway/cloud/CsvWriter.cpp
        include/gateway/cloud/CsvWriter.hpp)
TARGET_INCLUDE_DIRECTORIES(lora_gateway_lib PUBLIC "include/" "libs")
TARGET_LINK_LIBRARIES(lora_gateway_lib wiringPi ${Boost_LIBRARIES})

ADD_EXECUTABLE(LoraGateway main.cpp)
TARGET_LINK_LIBRARIES(LoraGateway lora_gateway_lib)
