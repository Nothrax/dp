import time
import logging

from uploader.device.device_type import DeviceType


class Device:
    def __init__(self, company_name: str, gateway_id: str, device_type: DeviceType, device_id: int):
        self.company_name = company_name
        self.gateway_id = gateway_id
        self.device_type = device_type
        self.device_id = device_id
        self._data_ask_timeout_s = 30
        self._last_data_read_time = 0
        self._last_data_read_id = -1
        self._logger = logging.getLogger("Device")
        return

    def is_read_data_timeout_reached(self) -> bool:
        current_time = time.time()
        is_timeout_reached = current_time - self._last_data_read_time > self._data_ask_timeout_s
        if self._last_data_read_time != 0 and is_timeout_reached:
            self._logger.warning(f"Data read timeout reached for device {self.device_type} {self.device_id} {self.company_name}")
        return is_timeout_reached

    def new_data_read_request(self, message_id: int) -> None:
        self._last_data_read_time = time.time()
        self._last_data_read_id = message_id

    def confirm_data_read(self, message_id: int) -> bool:
        if message_id == self._last_data_read_id:
            self._last_data_read_time = 0
            self._last_data_read_id = -1
            return True
        else:
            return False
