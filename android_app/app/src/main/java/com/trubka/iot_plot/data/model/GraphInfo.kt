package com.trubka.iot_plot.data.model

import com.trubka.iot_plot.api_connection.DeviceType
import java.util.*
import kotlin.collections.ArrayList

data class GraphInfo(
    val company: String,
    val deviceType: DeviceType,
    val deviceId: Int,
    val fields: ArrayList<String>,
    val timeFrom: Calendar,
    val timeTo: Calendar,
    val graphName: String,

    )


data class InfluxGraphs(
    val graphs: ArrayList<GraphInfo>
)