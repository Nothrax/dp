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
  }
}
```
- address - mqtt broker address
- port - mqtt broker port
- ssl - use ssl
- ca_cert - path to ca certificate
- cert_file - path to client certificate
- keyfile - path to client key