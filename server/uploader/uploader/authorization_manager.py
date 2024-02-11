from uploader.config import Config
from uploader.device.device_type import DeviceType


class GatewayRegistry:
    def __init__(self, company_name: str, gateway_id: str) -> None:
        self._company_name = company_name
        self._gateway_id = gateway_id
        self.registry = {}

    def add_device(self, device_type: DeviceType, device_id: int) -> None:
        if device_type not in self.registry:
            self.registry[device_type] = []
        self.registry[device_type].append(device_id)

    def has_device(self, device_type: DeviceType, device_id: int) -> bool:
        if device_type not in self.registry:
            return False
        return device_id in self.registry[device_type]


class CompanyGatewayRegistry:
    def __init__(self):
        self.registry = {}

    def add_device(self, company_name: str, gateway_id: str, device_type: DeviceType, device_id: int):
        if company_name not in self.registry:
            self.registry[company_name] = {}
        if gateway_id not in self.registry[company_name]:
            self.registry[company_name][gateway_id] = GatewayRegistry(company_name, gateway_id)
        self.registry[company_name][gateway_id].add_device(device_type, device_id)

    def get_gateway(self, company_name: str, gateway_id: str):
        if company_name not in self.registry:
            return None
        if gateway_id not in self.registry[company_name]:
            return None
        return self.registry[company_name][gateway_id]


class AuthorizationManager:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._company_device_registry = CompanyGatewayRegistry()
        return

    def load_authorization(self) -> None:
        # todo from relational database
        self._company_device_registry.add_device("company1", "gateway1", DeviceType.SCALE, 1)
        self._company_device_registry.add_device("company1", "gateway1", DeviceType.CELLAR, 1)
        return

    def get_gateway(self, company: str, gateway_id: str) -> GatewayRegistry:
        return self._company_device_registry.get_gateway(company, gateway_id)
