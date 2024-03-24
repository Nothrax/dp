from uploader.output_protocol.gateway_message import GatewayMessage
from uploader.config import Config
from uploader.device_manager import DeviceManager
from influxdb_client import InfluxDBClient, Point, WriteOptions, Bucket


class TimeseriesConnection:
    def __init__(self, token: str, organization: str, address: str) -> None:
        self._token = token
        self._organization = organization
        self._url = address
        self._client = InfluxDBClient(url=address, token=token, org=organization)
        self._write_api = self._client.write_api(
            write_options=WriteOptions(
                batch_size=2,
                flush_interval=10_000,
                jitter_interval=2_000,
                retry_interval=5_000,
                max_retries=5,
                max_retry_delay=30_000,
                exponential_base=2
            )
        )
        self._bucket_names = ["SCALE", "CELLAR"]
        self._create_buckets_if_not_exist()

    def _create_buckets_if_not_exist(self):
        buckets_api = self._client.buckets_api()

        for bucket_name in self._bucket_names:
            bucket = buckets_api.find_bucket_by_name(bucket_name)

            if bucket is None:
                buckets_api.create_bucket(bucket_name=bucket_name)
                print(f"Bucket '{bucket_name}' created successfully.")


    def write(self, point: Point, bucket: str) -> None:
        self._write_api.write(bucket=bucket, record=point)
        return


class TimeseriesManager:
    def __init__(self, config: Config, device_manager: DeviceManager) -> None:
        self._config = config
        self._device_manager = device_manager
        self._clients = {}
        return

    def store_data_points(self, message: GatewayMessage) -> None:
        gateway = self._device_manager.get_gateway(message.company, message.gateway_id)
        points = message.data_entry.get_influx_points()
        bucket = message.data_entry.get_bucket()

        if gateway.company_name not in self._clients:
            self._clients[gateway.company_name] = TimeseriesConnection(gateway.influx_token, gateway.company_name,
                                                                       self._config.influxdb_address)

        for point in points:
            self._clients[gateway.company_name].write(point, bucket)

        return
