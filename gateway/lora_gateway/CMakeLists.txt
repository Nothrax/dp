CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
SET(CMAKE_CXX_STANDARD 20)

PROJECT(LoraGateway)

SET(LoraGateway_VERSION 1.0.0)

SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(PahoMqttCpp REQUIRED)
FIND_PACKAGE(eclipse-paho-mqtt-c REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(Boost 1.79.0 REQUIRED COMPONENTS program_options json)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    INCLUDE_DIRECTORIES(libs/wiringpi_placeholder)
ENDIF()

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(lora_gateway_lib STATIC ${source_files})
TARGET_INCLUDE_DIRECTORIES(lora_gateway_lib PUBLIC "include/" "libs")


TARGET_LINK_LIBRARIES(lora_gateway_lib
        Boost::program_options
        Boost::json
        PahoMqttCpp::paho-mqttpp3
        eclipse-paho-mqtt-c::paho-mqtt3as
        OpenSSL::SSL
)

IF(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    TARGET_LINK_LIBRARIES(lora_gateway_lib
            wiringPi )
ENDIF()

ADD_EXECUTABLE(LoraGateway main.cpp)
TARGET_LINK_LIBRARIES(LoraGateway lora_gateway_lib)
