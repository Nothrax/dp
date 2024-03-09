package com.trubka.iot_plot.api_connection

import java.time.Instant
import java.util.*
import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL
import java.net.URLEncoder
import kotlin.collections.ArrayList


data class Value(val measurement: String, val value: Double, val time: Instant)

class ApiRepository {



    fun makeBucketListRequest(
        address: String, token: String, organization: String,
    ): ArrayList<String> {
/*        val influxDBClient =
            InfluxDBClientKotlinFactory.create(address, token.toCharArray(), organization)
        val query = "buckets()" +
                " |> drop(columns: [\"_stop\", \"time\", \"id\", \"organizationID\", \"retentionPolicy\", \"retentionPeriod\"])"
        val results = influxDBClient.getQueryKotlinApi().query(query)
        val list: ArrayList<String> = ArrayList()

        runBlocking { // this: CoroutineScope
            launch { // launch a new coroutine and continue
                results.consumeEach { list.add(it.values["name"] as String) }
            }
        }*/
        return ArrayList();
    }

    fun makeLocationListRequest(
        address: String, token: String, organization: String,
        bucket: String,
    ): ArrayList<String> {
        /*
        val influxDBClient =
            InfluxDBClientKotlinFactory.create(address, token.toCharArray(), organization)
        val query = "from(bucket: \"" + bucket + "\")\n" +
                "  |> range(start: 0, stop: 999999999999999999)\n" +
                "  |> drop(columns: [\"_start\", \"_stop\", \"_field\",\"_measurement\", \"_value\"])\n" +
                "  |> distinct(column: \"location\")"
        val list: ArrayList<String> = ArrayList()
        try {
            val results = influxDBClient.getQueryKotlinApi().query(query)
            runBlocking { // this: CoroutineScope
                launch { // launch a new coroutine and continue
                    results.consumeEach { list.add(it.values["_value"] as String) }
                }
            }
        } catch (e: java.lang.Exception) {
            // handler
        }
*/
        return ArrayList();
    }

    fun makeDeviceListRequest(
        address: String, token: String, organization: String,
        bucket: String,
        location: String
    ): ArrayList<String> {
        /*val influxDBClient =
            InfluxDBClientKotlinFactory.create(address, token.toCharArray(), organization)
        val query = "from(bucket: \"" + bucket + "\")\n" +
                "  |> range(start: 0, stop: 999999999999999999)\n" +
                "  |> drop(columns: [\"_field\", \"_start\", \"_stop\"])\n" +
                "  |> filter(fn: (r) => r[\"location\"] == \"" + location + "\")\n" +
                "  |> distinct(column: \"_measurement\")"
        val list: ArrayList<String> = ArrayList()
        try {
            val results = influxDBClient.getQueryKotlinApi().query(query)
            runBlocking { // this: CoroutineScope
                launch { // launch a new coroutine and continue
                    results.consumeEach { list.add(it.values["_value"] as String) }
                }
            }
        } catch (e: java.lang.Exception) {
            // handler
        }*/

        return ArrayList();
    }

    fun makeMeasurementListRequest(
        address: String, token: String, organization: String,
        bucket: String,
        location: String,
        device: String,
    ):ArrayList<String> {
       /* val influxDBClient =
            InfluxDBClientKotlinFactory.create(address, token.toCharArray(), organization)
        val query = "from(bucket: \"" + bucket + "\")\n" +
                "  |> range(start: 0, stop: 999999999999999999)\n" +
                "  |> filter(fn: (r) => r[\"location\"] == \"" + location + "\")\n" +
                "  |> filter(fn: (r) => r[\"_measurement\"] == \"" + device + "\")\n" +
                "  |> drop(columns: [\"_value\", \"_measurement\",\"_start\", \"_stop\", \"location\"])\n" +
                "  |> distinct(column: \"_field\")"
        val list: ArrayList<String> = ArrayList()
        try {
            val results = influxDBClient.getQueryKotlinApi().query(query)
            runBlocking { // this: CoroutineScope
                launch { // launch a new coroutine and continue
                    results.consumeEach { list.add(it.values["_value"] as String) }
                }
            }
        } catch (e: java.lang.Exception) {
            // handler
        }
*/

        return ArrayList()
    }

    fun makeGraphRequest(
        address: String, token: String, organization: String,
        bucket: String,
        location: String,
        device: String,
        measurements: List<String>,
        timeFrom: Calendar,
        timeTo: Calendar
    ): ArrayList<Value> {

        /*val list: ArrayList<Value> = ArrayList()

        if (measurements.isEmpty()) {
            return list
        } else {
            val startTimestamp: Long = timeFrom.getTime().time / 1000
            val endTimestamp: Long = timeTo.getTime().time / 1000
            val influxDBClient =
                InfluxDBClientKotlinFactory.create(address, token.toCharArray(), organization)
            var query = "from(bucket: \"" + bucket + "\")\n" +
                    "  |> range(start: " + startTimestamp + ", stop: " + endTimestamp + ")\n" +
                    "  |> filter(fn: (r) => r[\"_measurement\"] == \"" + device + "\")\n"

            query += "  |> filter(fn: (r) =>"
            query += " r[\"_field\"] == \"" + measurements[0] + "\" "
            val leftMeasurements = measurements.subList(1, measurements.lastIndex + 1)
            for (measurement in leftMeasurements) {
                query += "or  r[\"_field\"] == \"$measurement\" "
            }
            query += ")\n"
            query += "  |> filter(fn: (r) => r[\"location\"] == \"$location\")\n"

            try {
                val results = influxDBClient.getQueryKotlinApi().query(query)
                runBlocking {
                    launch {
                        results.consumeEach { list.add(Value(
                            it.values["_field"] as String,
                            it.values["_value"] as Double,
                            it.values["_time"] as Instant
                        )) }
                        val resultList = results.toList()
                        for (result in resultList) {
                            list.add(
                                Value(
                                    result.values["_field"] as String,
                                    result.values["_value"] as Double,
                                    result.values["_time"] as Instant
                                )
                            )
                        }
                    }
                }
            } catch (e: java.lang.Exception) {
                print("test")
            }
        }*/
        return ArrayList()
    }

    fun getCompanies(address: String, username: String, password: String): ArrayList<String> {
        return arrayListOf("jedna", "dba", "trzy")
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