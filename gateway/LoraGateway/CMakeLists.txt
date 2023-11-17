CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
SET(CMAKE_CXX_STANDARD 20)

PROJECT(LoraGateway)

SET(LoraGateway_VERSION 1.0.0)

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(lora_gateway_lib STATIC ${source_files})
TARGET_INCLUDE_DIRECTORIES(lora_gateway_lib PUBLIC "include/" "libs")
TARGET_LINK_LIBRARIES(lora_gateway_lib wiringPi)

ADD_EXECUTABLE(LoraGateway main.cpp)
TARGET_LINK_LIBRARIES(LoraGateway lora_gateway_lib)
