import json


class Config:
    def __init__(self, influxdb_address, db_address, db_user, db_password, flask_host, flask_port):
        self.influxdb_address = influxdb_address
        self.db_address = db_address
        self.db_user = db_user
        self.db_password = db_password
        self.flask_host = flask_host
        self.flask_port = flask_port


def load_config(json_file_path):
    try:
        with open(json_file_path, 'r') as json_file:
            data = json.load(json_file)

            flask_settings = data.get('flask_settings', {})
            flask_host = flask_settings.get('host')
            flask_port = flask_settings.get('port')

            timeseries_settings = data.get('timeseries_settings', {})
            influxdb_address = timeseries_settings.get('address')

            relation_settings = data.get('relation_settings', {})
            db_address = relation_settings.get('address')
            db_user = relation_settings.get('user')
            db_password = relation_settings.get('password')

            return Config(influxdb_address=influxdb_address, db_address=db_address, db_user=db_user,
                          db_password=db_password, flask_host=flask_host, flask_port=flask_port)
    except FileNotFoundError:
        print(f"Error: JSON file '{json_file_path}' not found.")
        return None
