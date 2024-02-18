import json


class Config:
    def __init__(self, address, port, ssl, ca_cert, cert_file, keyfile):
        self.address = address
        self.port = port
        self.ssl = ssl
        self.ca_cert = ca_cert
        self.cert_file = cert_file
        self.key_file = keyfile


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

            return Config(address, port, ssl, ca_cert, cert_file, keyfile)
    except FileNotFoundError:
        print(f"Error: JSON file '{json_file_path}' not found.")
        return None
