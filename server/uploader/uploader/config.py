import json


class Config:
    def __init__(self, address, port, ssl, ca_cert, cert_file, keyfile, influxdb_address,influxdb_token, db_address, db_user, db_password):
        self.mqtt_address = address
        self.mqtt_port = port
        self.mqtt_ssl = ssl
        self.mqtt_ca_cert = ca_cert
        self.mqtt_cert_file = cert_file
        self.mqtt_key_file = keyfile
        self.influxdb_address = influxdb_address
        self.influxdb_token = influxdb_token
        self.db_address = db_address
        self.db_user = db_user
        self.db_password = db_password


def load_config(json_file_path):
    try:
        with open(json_file_path, 'r') as json_file:
            data = json.load(json_file)
            mqtt_settings = data.get('mqtt_settings', {})

            address = mqtt_settings.get('address')
            port = mqtt_settings.get('port')
            ssl = mqtt_settings.get('ssl', False)
            ca_cert = mqtt_settings.get('ca_cert')
            cert_file = mqtt_settings.get('cert_file')
            keyfile = mqtt_settings.get('keyfile')

            timeseries_settings = data.get('timeseries_settings', {})
            influxdb_address = timeseries_settings.get('address')
            influxdb_token = timeseries_settings.get('token')

            relation_settings = data.get('relation_settings', {})
            db_address = relation_settings.get('address')
            db_user = relation_settings.get('user')
            db_password = relation_settings.get('password')

            return Config(address, port, ssl, ca_cert, cert_file, keyfile, influxdb_address, influxdb_token, db_address, db_user, db_password)
    except FileNotFoundError:
        print(f"Error: JSON file '{json_file_path}' not found.")
        return None
