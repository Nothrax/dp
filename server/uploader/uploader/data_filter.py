from uploader.device.device_type import DeviceType
class DataFilter:
    SCALE_MIN_TEMP = -60
    SCALE_MAX_TEMP = 150
    SCALE_MIN_HUMIDITY = 0
    SCALE_MAX_HUMIDITY = 100
    SCALE_MIN_WEIGHT = 0
    SCALE_MAX_WEIGHT = 500

    CELLAR_MIN_TEMP = -60
    CELLAR_MAX_TEMP = 150
    CELLAR_MIN_HUMIDITY = 0
    CELLAR_MAX_HUMIDITY = 100
    CELLAR_MIN_CO2 = 0
    CELLAR_MAX_CO2 = 100000

    def __init__(self):
        pass

    def values_are_valid(self, data_point, device_type: DeviceType):
        if device_type == DeviceType.SCALE:
            return self._scale_values_are_valid(data_point)
        elif device_type == DeviceType.CELLAR:
            return self._cellar_values_are_valid(data_point)

    def _scale_values_are_valid(self, data_point):
        if data_point.get("temperature") < self.SCALE_MIN_TEMP or data_point.get("temperature") > self.SCALE_MAX_TEMP:
            return False
        if data_point.get("humidity") < self.SCALE_MIN_HUMIDITY or data_point.get("humidity") > self.SCALE_MAX_HUMIDITY:
            return False
        if data_point.get("weight") < self.SCALE_MIN_WEIGHT or data_point.get("weight") > self.SCALE_MAX_WEIGHT:
            return False
        return True

    def _cellar_values_are_valid(self, data_point):
        if data_point.get("temperature") < self.CELLAR_MIN_TEMP or data_point.get("temperature") > self.CELLAR_MAX_TEMP:
            return False
        if data_point.get("humidity") < self.CELLAR_MIN_HUMIDITY or data_point.get(
                "humidity") > self.CELLAR_MAX_HUMIDITY:
            return False
        if data_point.get("co2") < self.CELLAR_MIN_CO2 or data_point.get("co2") > self.CELLAR_MAX_CO2:
            return False
        return True
