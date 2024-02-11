from uploader.config import Config


class CompanyDeviceRegistry:
    def __init__(self):
        # Initialize an empty dictionary to store company-device mappings
        self.registry = {}

    def add_device(self, company_name, device_id):
        # Add a device to the specified company's list
        if company_name not in self.registry:
            self.registry[company_name] = []
        self.registry[company_name].append(device_id)

    def has_device(self, company_name, device_id):
        # Check if a specific device exists for the given company
        return company_name in self.registry and device_id in self.registry[company_name]


class AuthorizationManager:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._company_device_registry = CompanyDeviceRegistry()
        return

    def load_authorization(self) -> None:
        # todo from relational datatbase
        self._company_device_registry.add_device("company1", "gateway1")
        return

    def is_authorized(self, company: str, gateway_id: str) -> bool:
        return self._company_device_registry.has_device(company, gateway_id)
