from time import sleep
import logging

from uploader.config import Config
from uploader.output_protocol import MqttClient, GatewayMessage, MessageType, UploaderMessage, ResponseType
from uploader.device_manager import DeviceManager
from uploader.device import Device
from uploader.timeseries_manager import TimeseriesManager


class Uploader:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._mqtt_client = MqttClient(config)
        self._running = False
        self._device_manager = DeviceManager(config)
        self._device_manager.load_devices()
        self._logger = logging.getLogger("Uploader")
        self._timeseries_manager = TimeseriesManager(config, self._device_manager)
        self._out_message_counter = 0
        return

    def start(self) -> None:
        self._mqtt_client.start()
        self._mqtt_client.connect()
        self._running = True
        self.listen()
        return

    def stop(self) -> None:
        self._mqtt_client.stop()
        self._running = False
        return

    def listen(self) -> None:
        while self._running:
            if not self._mqtt_client.is_connected():
                sleep(1)
                continue
            message = self._mqtt_client.get(timeout=1)
            if message is not None:
                self.handle_message(message)

    def handle_message(self, message: GatewayMessage) -> None:
        message.parse_message()
        device = self._device_manager.get_device(message.company, message.gateway_id, message.device_type,
                                                 message.device_id)
        if device is None:
            self.handle_unknown_device(message)
            return

        if message.message_type == MessageType.DATA:
            self.handle_data_message(message, device)
        elif message.message_type == MessageType.DATA_READ_RESPONSE:
            self.handle_data_read_response(message, device)
        else:
            self._logger.warning(f"Unsupported message type {message.message_type}")

    def handle_data_message(self, message: GatewayMessage, device: Device) -> None:
        # todo filter the data
        self._timeseries_manager.store_data_points(message)
        if message.stored_data_points:
            self.handle_stored_data(device)

        ack_message = UploaderMessage(MessageType.DATA_ACK, message.company, message.gateway_id, message.message_id)
        self._mqtt_client.publish(ack_message)
        return

    def handle_data_read_response(self, message: GatewayMessage, device: Device) -> None:
        if not device.confirm_data_read(message.message_id):
            self._logger.warning(f"Stored data received for device {device.device_type} {device.device_id} "
                            f"{device.company_name} with wrong message id {message.message_id}")
            return
        self._logger.info(f"Stored data received for device {device.device_type} {device.device_id} {device.company_name}")
        # todo filter the data
        self._timeseries_manager.store_data_points(message)
        ack_message = UploaderMessage(MessageType.DATA_READ_ACK, message.company, message.gateway_id,
                                      message.message_id)
        self._mqtt_client.publish(ack_message)
        return

    def handle_stored_data(self, device: Device) -> None:
        if device.is_read_data_timeout_reached():
            device.new_data_read_request(self._out_message_counter)
            data_read_message = UploaderMessage(MessageType.DATA_READ, device.company_name, device.gateway_id,
                                                self._out_message_counter)
            data_read_message.set_device(device.device_type, device.device_id)
            self._logger.info(f"Requesting stored data for device {device.device_type} {device.device_id} {device.company_name}, request id: {self._out_message_counter}")
            self._mqtt_client.publish(data_read_message)
            self._out_message_counter += 1
        return

    def handle_unknown_device(self, message: GatewayMessage) -> None:
        message_res_type = MessageType.DATA_ACK if message.message_type == MessageType.DATA else MessageType.DATA_READ_ACK
        uploader_message = UploaderMessage(message_res_type, message.company, message.gateway_id,
                                           message.message_id)
        uploader_message.set_response(ResponseType.ERROR, "Unknown company or gateway")
        self._mqtt_client.publish(uploader_message)
        return

