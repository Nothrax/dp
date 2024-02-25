import logging
import mysql.connector

from uploader.config import Config
from uploader.device import DeviceType, Device


class Gateway:
    def __init__(self, company_name: str, gateway_id: str, influx_token: str) -> None:
        self.company_name = company_name
        self.influx_token = influx_token
        self.gateway_id = gateway_id
        self.registry = {}

    def add_device(self, device_type: DeviceType, device_id: int) -> None:
        if device_type not in self.registry:
            self.registry[device_type] = {}
        self.registry[device_type][device_id] = Device(self.company_name, self.gateway_id, device_type, device_id)

    def get_device(self, device_type: DeviceType, device_id: int) -> Device | None:
        if device_type not in self.registry:
            return None
        if device_id not in self.registry[device_type]:
            return None
        return self.registry[device_type][device_id]


class CompanyRegistry:
    def __init__(self):
        self.registry = {}

    def add_device(self, company_name: str, gateway_id: str, device_type: DeviceType, device_id: int,
                   influx_token: str) -> None:
        if company_name not in self.registry:
            self.registry[company_name] = {}
        if gateway_id not in self.registry[company_name]:
            self.registry[company_name][gateway_id] = Gateway(company_name, gateway_id, influx_token)
        self.registry[company_name][gateway_id].add_device(device_type, device_id)

    def get_gateway(self, company_name: str, gateway_id: str) -> Gateway | None:
        if company_name not in self.registry:
            return None
        if gateway_id not in self.registry[company_name]:
            return None
        return self.registry[company_name][gateway_id]

    def get_company(self, company_name: str) -> Gateway | None:
        if company_name not in self.registry:
            return None
        return self.registry[company_name]


class DeviceManager:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._company_device_registry = CompanyRegistry()
        self._logger = logging.getLogger("DeviceManager")
        return

    def load_devices(self) -> None:
        # todo from relational database
        #self._company_device_registry.add_device("company1", "gateway1", DeviceType.SCALE, 1, self._config.influxdb_token)
        #self._company_device_registry.add_device("company1", "gateway1", DeviceType.CELLAR, 1, self._config.influxdb_token)
        # Connect to the database
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
            cursor.execute("""
                SELECT c.name AS company_name, g.name AS gateway_name, d.device_type, d.id AS device_id
                FROM device d
                INNER JOIN gateway g ON d.gateway_id = g.id
                INNER JOIN company c ON g.company_id = c.id
            """)

            devices = cursor.fetchall()

            number_of_devices = 0
            for device in devices:
                number_of_devices += 1
                self._company_device_registry.add_device(device['company_name'], device['gateway_name'], DeviceType(device['device_type']), device['device_id'], self._config.influxdb_token)
                print(f"registered device {DeviceType(device['device_type'])} {device['device_id']} from {device['company_name']} {device['gateway_name']}")
            print(f"Number of devices: {number_of_devices}")

        except mysql.connector.Error as err:
            print("Error executing query: {}".format(err))
            raise err

        cursor.close()
        conn.close()
        return

    def get_gateway(self, company: str, gateway_id: str) -> Gateway:
        return self._company_device_registry.get_gateway(company, gateway_id)

    def get_device(self, company_name: str, gateway_id: str, device_type: DeviceType, device_id: int) -> Device | None:
        gateway = self.get_gateway(company_name, gateway_id)
        if gateway is None:
            self._logger.warning(f"Unauthorized message from {company_name} {gateway_id}")
            return None
        device = gateway.get_device(device_type, device_id)
        if device is None:
            self._logger.warning(
                f"Unknown device {device_type} with id {device_id} from {company_name} {gateway_id}")
            return None
        return device
