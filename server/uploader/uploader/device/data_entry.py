from uploader.device.device_type import DeviceType
from influxdb_client import Point
import time


class DataEntry:
    def __init__(self, device_type: DeviceType, device_number: int, data_points) -> None:
        self._data_points = data_points
        self._device_type = device_type
        self._device_number = device_number

    def get_influx_points(self) -> list[Point] | None:
        if self._device_type == DeviceType.SCALE:
            return self.get_scale_influx_query()
        elif self._device_type == DeviceType.CELLAR:
            return self.get_cellar_influx_query()
        else:
            return []

    def get_scale_influx_query(self) -> list[Point] | None:
        points = []
        for data_point in self._data_points:
            temperature = data_point.get("temperature")
            humidity = data_point.get("humidity")
            co2 = data_point.get("weight")
            timestamp_ms = data_point.get("timestamp_ms")
            points.append(Point(str(self._device_number)).field("temperature", temperature)
                          .field("humidity", humidity)
                          .field("weight", co2)
                          .time(timestamp_ms, "ms"))
        return points

    def get_cellar_influx_query(self) -> list[Point] | None:
        points = []
        for data_point in self._data_points:
            temperature = data_point.get("temperature")
            humidity = data_point.get("humidity")
            co2 = data_point.get("co2")
            timestamp_ms = data_point.get("timestamp_ms")
            points.append(Point(str(self._device_number)).field("temperature", temperature)
                          .field("humidity", humidity)
                          .field("co2", co2)
                          .time(timestamp_ms, "ms"))
        return points

    def get_bucket(self) -> str:
        if self._device_type == DeviceType.SCALE:
            return "SCALE"
        elif self._device_type == DeviceType.CELLAR:
            return "CELLAR"
        else:
            return "unknown"
