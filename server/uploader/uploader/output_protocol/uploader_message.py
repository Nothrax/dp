import json

from uploader.output_protocol.message_type import MessageType
from uploader.device.device_type import DeviceType
from enum import Enum


class ResponseType(Enum):
    OK = 0
    ERROR = 1


class UploaderMessage:
    def __init__(self, message_type: MessageType, company_name: str, gateway_it: str, message_id: int) -> None:
        self._message_type = message_type
        self.company_name = company_name
        self.gateway_id = gateway_it
        self._response = ResponseType.OK
        self._message = ""
        self.message_id = message_id
        self._device_type = None
        self._device_id = None
        return

    def set_response(self, response: ResponseType, message: str) -> None:
        self._response = response
        self._message = message
        return

    def set_device(self, device_type: DeviceType, device_id: int) -> None:
        self._device_type = device_type
        self._device_id = device_id
        return

    def get_output_protocol_message(self) -> str:
        data_dict = {}
        if self._message_type == MessageType.DATA_ACK or self._message_type == MessageType.DATA_READ_ACK:
            data_dict = {
                "type": self._message_type.value,
                "id": self.message_id,
                "response_type": self._response.value,
                "error_data": self._message
            }
        elif self._message_type == MessageType.DATA_READ:
            data_dict = {
                "type": self._message_type.value,
                "id": self.message_id,
                "device_type": self._device_type.value,
                "device_id": self._device_id
            }
        return json.dumps(data_dict)
