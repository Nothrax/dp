from uploader.output_protocol.gateway_message import GatewayMessage
from uploader.config import Config


class TimeseriesManager:
    def __init__(self, config: Config) -> None:
        self._config = config
        return

    def store_data_points(self, message: GatewayMessage) -> None:
        # todo store data points
        return
