import os
import signal

from flask_endpoint.config import Config
import logging
from flask import Flask, request, jsonify


class FlaskEndpoint:
    def __init__(self, config: Config):
        self._config = config
        self._app = Flask(__name__)
        self._username = "test"
        self._password = "test"

        @self._app.route('/')
        def hello():
            return "Hello, world!"

        @self._app.route('/buckets', methods=['POST'])
        def get_buckets():
            try:
                # Get username and password from the request
                username = request.form.get('username')
                password = request.form.get('password')
                print(username)
                print(password)

                if username is None or password is None:
                    return jsonify({"error": "Username and password are required"}), 400
                if username != self._username or password != self._password:
                    return jsonify({"error": "Invalid username or password"}), 401

                buckets = ["bucket1", "bucket2", "bucket3"]

                return jsonify({"buckets": buckets})
            except Exception as e:
                return jsonify({"error": str(e)}), 500

        @self._app.route('/get_devices', methods=['POST'])
        def get_databases():
            try:
                # Get username and password from the request
                username = request.form.get('username')
                password = request.form.get('password')
                bucket = request.form.get('bucket')
                print(username)
                print(password)

                if username is None or password is None or bucket is None:
                    return jsonify({"error": "Username, password and bucket are required"}), 400
                if username != self._username or password != self._password:
                    return jsonify({"error": "Invalid username or password"}), 401

                databases_with_fields = []
                databases_with_fields.append({"device_type": "CELLAR", "fields": ["co2", "temperature", "humidity"], "devices": ["1", "2"]})
                databases_with_fields.append({"device_type": "SCALE", "fields": ["weight", "temperature", "humidity"], "devices": ["2"]})

                return jsonify({"databases": databases_with_fields})
            except Exception as e:
                return jsonify({"error": str(e)}), 500

    def start(self):
        logging.info("Starting FlaskEndpoint")
        self._app.run(host=self._config.flask_host, port=self._config.flask_port, debug=True)
        pass

    def stop(self):
        logging.info("Stopping FlaskEndpoint")
        pid = os.getpid()
        # Send a termination signal to the process
        os.kill(pid, signal.SIGINT)
        pass
