# LoRa Gateway
Gateway for LoRaWAN.

# Dependencies
[spdlog](https://github.com/gabime/spdlog) >= 1.12.0
[boost](https://www.boost.org/) >= 1.79.0
[wiringPi](http://wiringpi.com/) >= 2.46.0
[Paho MQTT c](https://github.com/eclipse/paho.mqtt.c.git) = 1.3.13
[Paho MQTT c++](https://github.com/eclipse/paho.mqtt.cpp) = 1.3.2

# Build
```
mkdir _build
cd _build
cmake ..
make
```

# Arguments
* `--config|-c <string>` path to json configuration file

# Config file
TODO