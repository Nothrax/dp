from time import sleep
import logging

from uploader.config import Config
from uploader.output_protocol.mqtt_client import MqttClient
from uploader.output_protocol.gateway_message import GatewayMessage
from uploader.authorization_manager import AuthorizationManager
from uploader.output_protocol.message_type import MessageType
from uploader.output_protocol.uploader_message import UploaderMessage, ResponseType


class Uploader:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._mqtt_client = MqttClient(config)
        self._running = False
        self._authorization_manager = AuthorizationManager(config)
        self._authorization_manager.load_authorization()
        self._logger = logging.getLogger("Uploader")
        return

    def start(self) -> None:
        self._mqtt_client.start()
        self._mqtt_client.connect()
        self._running = True
        while self._running:
            if not self._mqtt_client.is_connected():
                sleep(1)
                if not self._mqtt_client.connect():
                    continue
            message = self._mqtt_client.get(timeout=1)
            if message is not None:
                self.handle_message(message)
        return

    def stop(self) -> None:
        self._mqtt_client.stop()
        self._running = False
        return

    def handle_message(self, message: GatewayMessage) -> None:
        message.parse_message()

        if not self.validate_message(message):
            return

        if message.message_type == MessageType.DATA:
            self.handle_data_message(message)
        elif message.message_type == MessageType.DATA_READ_RESPONSE:
            self.handle_data_read_response(message)
        else:
            logging.warning(f"Unsupported message type {message.message_type}")

    def validate_message(self, message: GatewayMessage) -> bool:
        gateway = self._authorization_manager.get_gateway(message.company, message.gateway_id)
        authorized = True
        if gateway is None:
            self._logger.warning(f"Unauthorized message from {message.company} {message.gateway_id}")
            authorized = False
        if not gateway.has_device(message.device_type, message.device_id):
            self._logger.warning(
                f"Unknown device {message.device_type} with id {message.message_id} from {message.company} {message.gateway_id}")
            authorized = False
        if not authorized:
            message_type = MessageType.DATA_ACK if message.message_type == MessageType.DATA else MessageType.DATA_READ_ACK
            uploader_message = UploaderMessage(message_type, message.company, message.gateway_id, message.message_id)
            uploader_message.set_response(ResponseType.ERROR, "Unknown company or gateway")
            self._mqtt_client.publish(uploader_message)
        return authorized

    def handle_data_message(self, message: GatewayMessage) -> None:
        logging.info(
            f"Data message from {message.company} {message.gateway_id} with {message.device_type} {message.device_id} received")
        # todo filter the data
        # todo store the data
        # todo handle stored data points
        ack_message = UploaderMessage(MessageType.DATA_ACK, message.company, message.gateway_id, message.message_id)
        self._mqtt_client.publish(ack_message)
        return

    def handle_data_read_response(self, message: GatewayMessage) -> None:
        # todo work with the response
        # todo filter the data
        # todo store the data
        ack_message = UploaderMessage(MessageType.DATA_READ_ACK, message.company, message.gateway_id, message.message_id)
        self._mqtt_client.publish(ack_message)
        return
