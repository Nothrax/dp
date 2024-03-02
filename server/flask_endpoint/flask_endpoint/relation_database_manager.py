from typing import List, Dict

from flask_endpoint.config import Config
from flask_endpoint.device_type import DeviceType

import logging
import mysql.connector


class RelationDBManager:
    def __init__(self, config: Config):
        self._config = config
        self._logger = logging.getLogger("RelationDBManager")

    def is_authorized(self, username: str, password: str, company: str) -> bool:
        if username is None or password is None or company is None:
            return False
        companies = self.get_companies(username, password)
        if company not in companies:
            return False
        return True

    def get_companies(self, username: str, password: str) -> List[str]:
        companies = []
        try:
            conn = mysql.connector.connect(
                host=self._config.db_address,
                user=self._config.db_user,
                password=self._config.db_password,
                database="iot"
            )
        except mysql.connector.Error as err:
            print("Error connecting to MariaDB.")
            raise err

        cursor = conn.cursor(dictionary=True)

        try:
            cursor.execute(f"""SELECT c.name
                                FROM company c
                                JOIN employed e ON c.id = e.company_id
                                JOIN user u ON e.user_id = u.id
                                WHERE u.username = '{username}' AND u.password = '{password}';""")
            result = cursor.fetchall()

            for row in result:
                companies.append(row['name'])

        except mysql.connector.Error as err:
            print("Error executing query: {}".format(err))
            raise err

        cursor.close()
        conn.close()
        return companies

    def get_devices(self, company: str) -> List[Dict[str, List[str]]]:
        devices = []
        try:
            conn = mysql.connector.connect(
                host=self._config.db_address,
                user=self._config.db_user,
                password=self._config.db_password,
                database="iot"
            )
        except mysql.connector.Error as err:
            print("Error connecting to MariaDB.")
            raise err

        cursor = conn.cursor(dictionary=True)

        try:
            cursor.execute(f"""
                   SELECT c.name AS company_name, g.name AS gateway_name, d.device_type, d.device_name, d.id AS device_id
                   FROM device d
                   INNER JOIN gateway g ON d.gateway_id = g.id
                   INNER JOIN company c ON g.company_id = c.id
                   WHERE c.name = '{company}'
               """)

            result = cursor.fetchall()

            for row in result:
                if row['device_type'] == DeviceType.CELLAR.value:
                    devices.append({
                        "device_id": row['device_id'],
                        "device_type": row['device_type'],
                        "device_name": row['device_name'],
                        "fields": ["temperature", "humidity", "co2"]})
                elif row['device_type'] == DeviceType.SCALE.value:
                    devices.append({
                        "device_id": row['device_id'],
                        "device_type": row['device_type'],
                        "device_name": row['device_name'],
                        "fields": ["weight", "temperature", "humidity"]})

        except mysql.connector.Error as err:
            print("Error executing query: {}".format(err))
            raise err

        cursor.close()
        conn.close()
        return devices
