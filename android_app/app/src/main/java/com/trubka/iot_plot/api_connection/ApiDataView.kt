package com.trubka.iot_plot.api_connection

data class ApiDataView(
    val apiAddress: String? = null,
    val password: String? = null,
    val username: String? = null,
    val bucketList: ArrayList<String>? = null,
    val locationList: ArrayList<String>? = null,
    val deviceList: ArrayList<String>? = null,
    val measurementList: ArrayList<String>? = null,
    val dataList: ArrayList<Value>? = null
)