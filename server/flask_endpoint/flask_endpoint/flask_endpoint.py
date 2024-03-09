import os
import signal
import logging

from flask_endpoint.config import Config
from flask_endpoint.influxdb_manager import InfluxDBConnection
from flask_endpoint.relation_database_manager import RelationDBManager
from flask_endpoint.device_type import DeviceType
import logging
from flask import Flask, request, jsonify


class FlaskEndpoint:
    def __init__(self, config: Config):
        self._config = config
        self._app = Flask(__name__)
        self._logger = logging.getLogger("FlaskEndpoint")
        self._influxdb_manager = InfluxDBConnection(config)
        self._relation_db_manager = RelationDBManager(config)

        @self._app.route('/get_companies', methods=['POST'])
        def get_companies():
            username = request.form.get('username')
            password = request.form.get('password')
            if not self._relation_db_manager.is_authenticated(username, password):
                return jsonify({"error": "Wrong password or username"}), 401

            companies = self._relation_db_manager.get_companies(username, password)
            return jsonify(companies)

        @self._app.route('/get_buckets', methods=['POST'])
        def get_buckets():
            try:
                # Get username and password from the request
                username = request.form.get('username')
                password = request.form.get('password')
                company = request.form.get('company')

                if not self._relation_db_manager.is_authorized(username, password, company):
                    return jsonify({"error": "Invalid username or password"}), 401

                self._logger.info("Get_buckets request from user {}".format(username))

                buckets = self._influxdb_manager.get_buckets(company)

                return jsonify({"buckets": buckets})
            except Exception as e:
                return jsonify({"error": str(e)}), 500

        @self._app.route('/get_devices', methods=['POST'])
        def get_devices():
            try:
                # Get username and password from the request
                username = request.form.get('username')
                password = request.form.get('password')
                company = request.form.get('company')

                if not self._relation_db_manager.is_authorized(username, password, company):
                    return jsonify({"error": "Invalid username or password"}), 401

                self._logger.info("Get_devices request from user {} and company {}".format(username, company))

                devices = self._relation_db_manager.get_devices(company)

                return jsonify(devices)
            except Exception as e:
                return jsonify({"error": str(e)}), 500

        @self._app.route('/get_data', methods=['POST'])
        def get_data():
            try:
                # Get username and password from the request
                username = request.form.get('username')
                password = request.form.get('password')
                company = request.form.get('company')
                device_type = int(request.form.get('device_type'))
                device_id = int(request.form.get('device_id'))
                fields = request.form.getlist('fields')
                time_from = request.form.get('from')
                time_to = request.form.get('to')

                if not self._relation_db_manager.is_authorized(username, password, company):
                    return jsonify({"error": "Invalid username or password"}), 401

                if device_type is None or device_id is None or fields is None or time_from is None or time_to is None:
                    return jsonify(
                        {"error": "Username, password, bucket, device_type, device and fields are required"}), 400

                self._logger.info(
                    "Get_data request from user {} and company {} for device {} and fields {}".format(username, company,
                                                                                                      device_id,
                                                                                                      fields))

                data = self._influxdb_manager.get_data(company, device_id, DeviceType(device_type), time_from, time_to,
                                                       fields)

                return jsonify(data)
            except Exception as e:
                return jsonify({"error": str(e)}), 500

    def start(self):
        logging.info("Starting FlaskEndpoint")
        self._app.run(host=self._config.flask_host, port=self._config.flask_port)
        pass

    def stop(self):
        logging.info("Stopping FlaskEndpoint")
        pid = os.getpid()
        # Send a termination signal to the process
        os.kill(pid, signal.SIGINT)
        pass
