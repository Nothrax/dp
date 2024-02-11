# Uploader
Application implements output protocol for getting IoT aggriculture data, authorizes the source and stores filtered data in influxDB.

# Run
```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python3 main.py --config ./config/default.json
```