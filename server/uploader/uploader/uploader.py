from time import sleep
import logging

from uploader.config import Config
from uploader.output_protocol.mqtt_client import MqttClient
from uploader.output_protocol.message import Message
from uploader.authorization_manager import AuthorizationManager


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

    def handle_message(self, message: Message) -> None:
        if not self._authorization_manager.is_authorized(message.company, message.gateway_id):
            self._logger.warning(f"Unauthorized message: {message.get_output_protocol_message()} from {message.company} {message.gateway_id}")
            return
        #todo different type of messages
        print(f"Handling message: {message}")
        #todo filter
        #todo upload to database
        return