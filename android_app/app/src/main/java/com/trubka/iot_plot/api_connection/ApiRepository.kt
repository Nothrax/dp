package com.trubka.iot_plot.api_connection

import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import kotlinx.serialization.ExperimentalSerializationApi
import kotlinx.serialization.Serializable
import kotlinx.serialization.decodeFromString
import java.time.Instant
import java.util.*
import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL
import java.net.URLEncoder
import kotlin.collections.ArrayList
import kotlinx.serialization.json.Json
import kotlinx.serialization.json.Json.Default.decodeFromString
import kotlinx.serialization.json.JsonArray
import kotlinx.serialization.json.JsonElement
import kotlinx.serialization.json.double
import kotlinx.serialization.json.int
import kotlinx.serialization.json.jsonArray
import kotlinx.serialization.json.jsonObject
import kotlinx.serialization.json.jsonPrimitive
import kotlinx.serialization.json.long

data class Value(val value: Double, val time: Instant)
data class Measurement(val measurement: String, val unit: String, val values: List<Value>)
@Serializable
data class Company(val name: String)

class ApiRepository {
    @OptIn(ExperimentalSerializationApi::class)
    fun makeDeviceListRequest(
        address: String, username: String, password: String,
        company: String
    ): ArrayList<Device> {
        try {
            val url = URL("http://$address/get_devices")
            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "POST"
            connection.doOutput = true

            // Set request parameters (form data)
            val postData = "username=${URLEncoder.encode(username, "UTF-8")}&password=${URLEncoder.encode(password, "UTF-8")}&company=${URLEncoder.encode(company, "UTF-8")}"
            val postDataBytes = postData.toByteArray(Charsets.UTF_8)

            // Send the request
            val outputStream = DataOutputStream(connection.outputStream)
            outputStream.write(postDataBytes)
            outputStream.flush()
            outputStream.close()

            // Get the response
            val responseCode = connection.responseCode
            println("\nSent 'POST' request to URL: $url; Response Code: $responseCode")

            if (responseCode == HttpURLConnection.HTTP_OK) {
                val reader = BufferedReader(InputStreamReader(connection.inputStream))
                val response = StringBuilder()
                var line: String?
                while (reader.readLine().also { line = it } != null) {
                    response.append(line)
                }
                reader.close()
                val jsonString: String = response.toString()

                //val devices = Json.decodeFromString(jsonString)
                val deviceList: ArrayList<Device> = ArrayList()
                val devices = Json.parseToJsonElement(jsonString)
                for (device in devices.jsonArray) {
                    val deviceMap = device.jsonObject
                    val fields = deviceMap["fields"]?.jsonArray
                    val fieldsList: ArrayList<String> = ArrayList()
                    if (fields != null) {
                        for (field in fields) {
                            fieldsList.add(field.jsonPrimitive.content)
                        }
                    }
                    val deviceObject = Device(
                        deviceId = deviceMap["device_id"]!!.jsonPrimitive.int,
                        deviceName = deviceMap["device_name"]!!.jsonPrimitive.content,
                        deviceType = DeviceType.fromInt(deviceMap["device_type"]!!.jsonPrimitive.int),
                        fields = fieldsList
                    )
                    deviceList.add(deviceObject)
                }
                return deviceList

            } else {
                println("Error: Unable to connect to the server.")
                return ArrayList()
            }
        } catch (e: Exception) {
            println("Exception occurred: ${e.message}")
        }
        return ArrayList()
    }

    fun makeGraphRequest(
        address: String, username: String, password: String,
        company: String,
        deviceType: DeviceType,
        deviceId: Int,
        fields: List<String>,
        timeFrom: Calendar,
        timeTo: Calendar
    ): ArrayList<Measurement> {
        try {
            val url = URL("http://$address/get_data")
            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "POST"
            connection.doOutput = true

            // Set request parameters (form data)
            val startTimestamp: Long = timeFrom.time.time / 1000
            val endTimestamp: Long = timeTo.time.time / 1000

            val postData = "username=${URLEncoder.encode(username, "UTF-8")}" +
                    "&password=${URLEncoder.encode(password, "UTF-8")}" +
                    "&device_type=${URLEncoder.encode(String.format("%d", deviceType.value), "UTF-8")}" +
                    "&device_id=${URLEncoder.encode(String.format("%d",deviceId), "UTF-8")}" +
                    "&company=${URLEncoder.encode(company, "UTF-8")}" +
                    "&from=${URLEncoder.encode(String.format("%d",startTimestamp), "UTF-8")}" +
                    "&to=${URLEncoder.encode(String.format("%d",endTimestamp), "UTF-8")}" +
                    "&fields=${URLEncoder.encode(fields.joinToString(","), "UTF-8")}" // Add the fields parameter
            val postDataBytes = postData.toByteArray(Charsets.UTF_8)

            // Send the request
            val outputStream = DataOutputStream(connection.outputStream)
            outputStream.write(postDataBytes)
            outputStream.flush()
            outputStream.close()

            // Get the response
            val responseCode = connection.responseCode
            println("\nSent 'POST' request to URL: $url; Response Code: $responseCode")

            if (responseCode == HttpURLConnection.HTTP_OK) {
                val reader = BufferedReader(InputStreamReader(connection.inputStream))
                val response = StringBuilder()
                var line: String?
                while (reader.readLine().also { line = it } != null) {
                    response.append(line)
                }
                reader.close()
                val jsonString: String = response.toString()
                val measurementList: ArrayList<Measurement> = ArrayList()
                val measurements = Json.parseToJsonElement(jsonString)
                for (measurement in measurements.jsonArray) {
                    val deviceMap = measurement.jsonObject
                    val values = deviceMap["values"]?.jsonArray
                    val valuesList: ArrayList<Value> = ArrayList()
                    if (values != null) {
                        for (value in values) {
                            val valueMap = value.jsonObject
                            val valueObject = Value(
                                value = valueMap["value"]!!.jsonPrimitive.double,
                                time = Instant.ofEpochSecond(valueMap["time"]!!.jsonPrimitive.long)
                            )
                            valuesList.add(valueObject)
                        }
                    }
                    val measurementObject = Measurement(
                        measurement = deviceMap["measurement"]!!.jsonPrimitive.content,
                        unit = deviceMap["unit"]!!.jsonPrimitive.content,
                        values = valuesList
                    )
                    measurementList.add(measurementObject)
                }

                return measurementList
            } else {
                println("Error: Unable to connect to the server.")
                return ArrayList()
            }
        } catch (e: Exception) {
            println("Exception occurred: ${e.message}")
        }
        return ArrayList()
    }

    @OptIn(ExperimentalSerializationApi::class)
    fun getCompanies(address: String, username: String, password: String): ArrayList<String> {
        try {
            val url = URL("http://$address/get_companies")
            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "POST"
            connection.doOutput = true

            // Set request parameters (form data)
            val postData = "username=${URLEncoder.encode(username, "UTF-8")}&password=${URLEncoder.encode(password, "UTF-8")}"
            val postDataBytes = postData.toByteArray(Charsets.UTF_8)

            // Send the request
            val outputStream = DataOutputStream(connection.outputStream)
            outputStream.write(postDataBytes)
            outputStream.flush()
            outputStream.close()

            // Get the response
            val responseCode = connection.responseCode
            println("\nSent 'POST' request to URL: $url; Response Code: $responseCode")

            if (responseCode == HttpURLConnection.HTTP_OK) {
                val reader = BufferedReader(InputStreamReader(connection.inputStream))
                val response = StringBuilder()
                var line: String?
                while (reader.readLine().also { line = it } != null) {
                    response.append(line)
                }
                reader.close()
                val jsonString: String = response.toString()
                val companies: List<String> = Json.decodeFromString(jsonString)

                return companies as ArrayList<String>
            } else {
                println("Error: Unable to connect to the server.")
                return ArrayList()
            }
        } catch (e: Exception) {
            println("Exception occurred: ${e.message}")
            return ArrayList()
        }
    }

    fun makeLoginRequest(address: String, username: String, password: String): Boolean {
        try {
            val url = URL("http://$address/get_companies")

            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "POST"
            connection.doOutput = true

            // Set request parameters (form data)
            val postData = "username=${URLEncoder.encode(username, "UTF-8")}&password=${URLEncoder.encode(password, "UTF-8")}"
            val postDataBytes = postData.toByteArray(Charsets.UTF_8)

            // Send the request
            val outputStream = DataOutputStream(connection.outputStream)
            outputStream.write(postDataBytes)
            outputStream.flush()
            outputStream.close()

            // Get the response
            val responseCode = connection.responseCode
            println("\nSent 'POST' request to URL: $url; Response Code: $responseCode")

            if (responseCode == HttpURLConnection.HTTP_OK) {
                val reader = BufferedReader(InputStreamReader(connection.inputStream))
                var line: String?
                while (reader.readLine().also { line = it } != null) {
                    println(line)
                }
                reader.close()
                return true
            } else {
                println("Error: Unable to connect to the server.")
                return false
            }
        } catch (e: Exception) {
            println("Exception occurred: ${e.message}")
            return false
        }
    }

}