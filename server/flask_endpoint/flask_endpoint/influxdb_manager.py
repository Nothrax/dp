from typing import List, Dict, Any

from influxdb_client import InfluxDBClient
from flask_endpoint.config import Config
from flask_endpoint.device_type import DeviceType, get_unit_for_field


class InfluxDBConnection:
    def __init__(self, config: Config) -> None:
        self._config = config
        self._client = InfluxDBClient(url=config.influxdb_address, token=config.influxdb_token)
        self._query_api = self._client.query_api()

        return

    def get_buckets(self, company: str) -> list[str]:
        result = self._query_api.query(
            'buckets() |> drop(columns: ["_stop", "time", "id", "organizationID", "retentionPolicy", "retentionPeriod"])',
            org=company)
        buckets = []
        for table in result:
            for record in table.records:
                bucket_name = record.values['name']
                if not bucket_name.startswith('_'):
                    buckets.append(bucket_name)
        return buckets

    def get_data(self, company: str, device_id: int, device_type: DeviceType, start_time: str, end_time: str,
                 fields: list[str]):
        data = []
        if device_type == DeviceType.SCALE:
            bucket = "SCALE"
        elif device_type == DeviceType.CELLAR:
            bucket = "CELLAR"
        else:
            return data
        query = f'from(bucket: "{bucket}") |> range(start: {start_time}, stop: {end_time}) |> filter(fn: (r) => r["_measurement"] == "{device_id}")'
        result = self._query_api.query(query, org=company)

        for table in result:
            data_set = []
            if len(table.records) == 0:
                continue
            if table.records[0].values["_field"] not in fields:
                continue
            field = table.records[0].values["_field"]
            for record in table:
                time = record.values["_time"]
                data_point = {"time": int(time.timestamp()), "value": record.values["_value"]}
                data_set.append(data_point)
            data.append({"measurement": field, "unit": get_unit_for_field(field, device_type), "values": data_set})
        return data
