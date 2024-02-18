import logging

from uploader.config import Config
from uploader.device import DeviceType, Device


class GatewayRegistry:
    def __init__(self, company_name: str, gateway_id: str) -> None:
        self._company_name = company_name
        self._gateway_id = gateway_id
        self.registry = {}

    def add_device(self, device_type: DeviceType, device_id: int) -> None:
        if device_type not in self.registry:
            self.registry[device_type] = {}
        self.registry[device_type][device_id] = Device(self._company_name, self._gateway_id, device_type, device_id)

    def get_device(self, device_type: DeviceType, device_id: int) -> Device | None:
        if device_type not in self.registry:
            return None
        if device_id not in self.registry[device_type]:
            return None
        return self.registry[device_type][device_id]


class CompanyGatewayRegistry:
    def __init__(self):
        self.registry = {}

    def add_device(self, company_name: str, gateway_id: str, device_type: DeviceType, device_id: int) -> None:
        if company_name not in self.registry:
            self.registry[company_name] = {}
        if gateway_id not in self.registry[company_name]:
            self.registry[company_name][gateway_id] = GatewayRegistry(company_name, gateway_id)
        self.registry[company_name][gateway_id].add_device(device_type, device_id)

    def get_gateway(self, company_name: str, gateway_id: str) -> GatewayRegistry | None:
        if company_name not in self.registry:
            return None
        if gateway_id not in self.registry[company_name]:
            return None
        return self.registry[company_name][gateway_id]


class DeviceManager:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._company_device_registry = CompanyGatewayRegistry()
        self._logger = logging.getLogger("DeviceManager")
        return

    def load_devices(self) -> None:
        # todo from relational database
        self._company_device_registry.add_device("company1", "gateway1", DeviceType.SCALE, 1)
        self._company_device_registry.add_device("company1", "gateway1", DeviceType.CELLAR, 1)
        return

    def get_gateway(self, company: str, gateway_id: str) -> GatewayRegistry:
        return self._company_device_registry.get_gateway(company, gateway_id)

    def get_device(self, company_name: str, gateway_id: str, device_type: DeviceType, device_id: int) -> Device | None:
        gateway = self.get_gateway(company_name, gateway_id)
        if gateway is None:
            self._logger.warning(f"Unauthorized message from {company_name} {gateway_id}")
        device = gateway.get_device(device_type, device_id)
        if device is None:
            self._logger.warning(
                f"Unknown device {device_type} with id {device_id} from {company_name} {gateway_id}")
        return device
