# Uploader
Application implements flask endpoints for getting IoT aggriculture data from influxDB.

# Endpoints
- /get_data - get data from influxDB
- /get_devices - get devices for user
- /get_compaines - get companies for user

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
  "flask_settings": {
    "host": "127.0.0.1",
    "port": 5000
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
- flask_settings.host - hostname for flask service
- flask_settings.port - port for flask service
- timeseries_settings.address - address for influxDB
- timeseries_settings.token - token for influxDB
- relation_settings.address - address for relationDB
- relation_settings.user - user for relationDB
- relation_settings.password - password for relationDB