import logging
import ssl
import paho.mqtt.client as mqtt
from queue import Queue, Empty

from uploader.output_protocol import GatewayMessage
from uploader.output_protocol import UploaderMessage
from uploader.config import Config


class MqttClient:
    def __init__(self, config: Config) -> None:
        self._logger = logging.getLogger(self.__class__.__name__)
        self._config = config
        self._received_msgs: Queue[GatewayMessage] = Queue()
        self._mqtt_client = mqtt.Client(
            client_id="uploader",
            protocol=mqtt.MQTTv311,
            callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
        )
        self._is_connected = False
        self._mqtt_client.on_connect = self._on_connect
        self._mqtt_client.on_disconnect = self._on_disconnect
        self._mqtt_client.on_message = self._on_message
        if self._config.mqtt_ssl:
            self._mqtt_client.tls_set(
                ca_certs=self._config.mqtt_ca_cert,
                certfile=self._config.mqtt_cert_file,
                keyfile=self._config.mqtt_key_file,
                tls_version=ssl.PROTOCOL_TLS_CLIENT,
            )
            self._mqtt_client.tls_insecure_set(False)

    def _on_connect(self, client, _userdata, _flags, _rc, _properties):
        self._is_connected = True
        self._logger.info("Server connected to MQTT broker")
        client.subscribe("#", qos=0)

    def _on_disconnect(self, client, userdata, flags, reason_code, properties) -> None:
        self._is_connected = False
        self._logger.info("Server disconnected from MQTT broker")

    def _on_message(self, _client: mqtt.Client, _userdata, mqtt_message: mqtt.MQTTMessage) -> None:
        split_topic = mqtt_message.topic.split("/")
        if len(split_topic) != 3:
            return
        company, gateway_id, direction = split_topic
        if direction == "gateway":
            message = GatewayMessage(company, gateway_id, mqtt_message.payload.decode("utf-8"))
            self._received_msgs.put(message)

    def connect(self) -> bool:
        try:
            self._mqtt_client.connect(self._config.mqtt_address, port=self._config.mqtt_port, keepalive=60)
            return True
        except ConnectionRefusedError:
            self._logger.error(f"Connection refused to {self._config.mqtt_address}:{self._config.mqtt_port}")
            return False

    def start(self) -> None:
        self._mqtt_client.loop_start()

    def stop(self) -> None:
        self._mqtt_client.loop_stop()
        self._is_connected = False

    def publish(self, message: UploaderMessage) -> None:
        topic = f"{message.company_name}/{message.gateway_id}/uploader"
        self._mqtt_client.publish(topic=topic, payload=message.get_output_protocol_message(), qos=0,
                                  retain=False)

    def get(self, timeout: int | None = None) -> GatewayMessage | None:
        try:
            return self._received_msgs.get(block=True, timeout=timeout)
        except Empty:
            return None

    def is_connected(self) -> bool:
        return self._is_connected
