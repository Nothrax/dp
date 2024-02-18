import logging
import json
from uploader.output_protocol.message_type import MessageType
from uploader.device.device_type import DeviceType


class GatewayMessage:
    def __init__(self, company: str, gateway_id: str, json_message: str) -> None:
        self._data_points = None
        self._json_message = json_message
        self._logger = logging.getLogger("Message")
        self.company = company
        self.gateway_id = gateway_id
        self.message_type = None
        self.device_type = None
        self.device_id = None
        self.message_id = None
        self.stored_data_points = False
        return

    def parse_message(self) -> None:
        parsed_data = json.loads(self._json_message)
        self.message_type = MessageType(parsed_data.get("type"))
        self.message_id = parsed_data.get("id")
        self.device_type = DeviceType(parsed_data.get("device_type"))
        self.device_id = parsed_data.get("device_id")
        self.stored_data_points = parsed_data.get("stored_data_points")
        self._data_points = parsed_data.get("data_points")
        self._logger.info(
            f"Message received from company: {self.company}, "
            f"gateway {self.gateway_id}, "
            f"message type: {self.message_type}, "
            f"device type: {self.device_type}, "
            f"device id: {self.device_id}, "
            f"message id: {self.message_id}, "
            f"stored data points: {self.stored_data_points}")
