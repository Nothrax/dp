from enum import Enum


class DeviceType(Enum):
    CELLAR = 1
    SCALE = 2


def get_unit_for_field(field: str, device_type: DeviceType) -> str:
    if device_type == DeviceType.SCALE:
        if field == "weight":
            return "kg"
        elif field == "temperature":
            return "°C"
        elif field == "humidity":
            return "%"
        else:
            return "unknown"
    elif device_type == DeviceType.CELLAR:
        if field == "temperature":
            return "°C"
        elif field == "humidity":
            return "%"
        elif field == "co2":
            return "ppm"
        else:
            return "unknown"
    else:
        return "unknown"
