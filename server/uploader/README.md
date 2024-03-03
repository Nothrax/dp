# Uploader
Application implements output protocol for getting IoT aggriculture data, authorizes the source and stores filtered data in influxDB.

# Run
```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 main.py --config ./config/default.json
```

# Config
```json
{
  "mqtt_settings": {
    "address": "127.0.0.1",
    "port": 1883,
    "ssl": false,
    "ca_cert": "./ca.pem",
    "cert_file": "./cert.pem",
    "keyfile": "key.pem"
  },
  "timeseries_settings": {
    "address": "127.0.0.1:8042",
    "token": "randomTokenValue"
  },
  "relation_settings": {
    "address": "127.0.0.1",
    "user": "root",
    "password": "example"
  }
}
```
- mqtt_settings.address - mqtt broker address
- mqtt_settings.port - mqtt broker port
- mqtt_settings.ssl - use ssl
- mqtt_settings.ca_cert - path to ca certificate
- mqtt_settings.cert_file - path to certificate
- mqtt_settings.keyfile - path to keyfile
- timeseries_settings.address - influxDB address
- timeseries_settings.token - influxDB token
- relation_settings.address - relation database address
- relation_settings.user - relation database user
- relation_settings.password - relation database password