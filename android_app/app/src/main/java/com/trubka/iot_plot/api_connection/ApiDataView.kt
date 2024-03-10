package com.trubka.iot_plot.api_connection

enum class DeviceType(val value: Int){
    CELLAR(1),
    SCALE(2);
    companion object {
        fun fromInt(value: Int): DeviceType =
            values().first { it.value == value }
    }
}

fun enumToString(deviceType: DeviceType): String {
    return deviceType.name
}

fun stringToEnum(deviceType: String): DeviceType {
    return DeviceType.valueOf(deviceType)
}

data class Device(
    val deviceId: Int,
    val deviceName: String,
    val deviceType: DeviceType,
    val fields: ArrayList<String>
)
data class ApiDataView(
    val apiAddress: String? = null,
    val password: String? = null,
    val username: String? = null,
    val companyList: ArrayList<String>? = null,
    val deviceList: ArrayList<Device>? = null,
    val currentDeviceType: DeviceType? = null,
    val measurements: ArrayList<Measurement>? = null
)

