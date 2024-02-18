# LoRa Gateway

Gateway for LoRaWAN.

# Dependencies

[spdlog](https://github.com/gabime/spdlog) >= 1.12.0
[boost](https://www.boost.org/) >= 1.79.0
[wiringPi](http://wiringpi.com/) >= 2.46.0
[Paho MQTT c](https://github.com/eclipse/paho.mqtt.c.git) = 1.3.13
[Paho MQTT c++](https://github.com/eclipse/paho.mqtt.cpp) = 1.3.2
fmt

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

``` json
{
  "generic_settings": {
    "verbose": true,
    "log_path": "./log",
    "gateway_id": "gateway1",
    "company": "company"
  },
  "device_settings": {
    "supported_devices": [
      {
        "device_type": 1,
        "device_number": 1
      },
      {
        "device_type": 2,
        "device_number": 1
      }
    ],
    "device_communication_type": "LORA",
    "generator_settings": {
      "device_type": 1,
      "device_number": 1
    },
    "lora_settings": {
      "uart_device_path": "/dev/ttyS0",
      "lora_channel": 2,
      "m0_pin": 4,
      "m1_pin": 5,
      "uart_baudrate": 9600,
      "lora_address": 5156
    }
  },
  "output_settings": {
    "output_type": "MQTT",
    "csv_path": "/home/nothrax/data",
    "mqtt_settings": {
      "hostname": "localhost",
      "port": 1883,
      "username": "test1",
      "password": "test2",
      "ssl": false,
      "ca_file": "",
      "client_cert": "",
      "client_key": ""
    }
  }
}
```
- `generic_settings`:
  - `verbose`: if true, log will be printed to stdout
  - `log_path`: path to log file
  - `gateway_id`: gateway id
  - `company`: company name
- `device_settings`:
  - `supported_devices`: list of supported devices
    - `device_type`: device type
    - `device_number`: device number
  - `device_communication_type`: communication type, LORA or GENERATOR
  - `generator_settings`: settings for generator
    - `device_type`: device type
    - `device_number`: device number
  - `lora_settings`: settings for lora
    - `uart_device_path`: path to uart device
    - `lora_channel`: lora channel
    - `m0_pin`: m0 pin
    - `m1_pin`: m1 pin
    - `uart_baudrate`: uart baudrate
    - `lora_address`: lora address
- `output_settings`:
  - `output_type`: output type, MQTT or CSV
  - `csv_path`: path to csv file
  - `mqtt_settings`: settings for mqtt
    - `hostname`: mqtt hostname
    - `port`: mqtt port
    - `username`: mqtt username
    - `password`: mqtt password
    - `ssl`: if true, use ssl
    - `ca_file`: path to ca file
    - `client_cert`: path to client certificate
    - `client_key`: path to client key

# Docker

Build docker `docker build -t gateway .` and run
it `docker run --rm --network host gateway /gateway/docker_build/LoraGateway --config=/gateway/resources/configuration/for_docker.json`