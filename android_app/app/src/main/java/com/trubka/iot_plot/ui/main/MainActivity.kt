package com.trubka.iot_plot.ui.main

import android.app.DatePickerDialog
import android.app.TimePickerDialog
import android.content.Context
import android.graphics.Color
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.trubka.iot_plot.R
import com.trubka.iot_plot.databinding.ActivityMainBinding
import com.trubka.iot_plot.api_connection.ApiDataView
import com.trubka.iot_plot.data.model.InfluxGraphs
import com.trubka.iot_plot.api_connection.ApiViewModel
import com.trubka.iot_plot.api_connection.ApiViewModelFactory
import java.text.SimpleDateFormat
import java.util.*
import kotlin.collections.ArrayList
import android.content.SharedPreferences
import com.trubka.iot_plot.data.model.GraphInfo
import com.google.gson.Gson
import com.jjoe64.graphview.GraphView
import com.jjoe64.graphview.LegendRenderer
import com.jjoe64.graphview.helper.DateAsXAxisLabelFormatter
import com.jjoe64.graphview.series.DataPoint
import com.jjoe64.graphview.series.LineGraphSeries
import android.os.StrictMode
import android.os.StrictMode.ThreadPolicy


class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var graphSpinner: Spinner
    private lateinit var chooserView: LinearLayout
    private lateinit var addView: LinearLayout
    private lateinit var graphView: LinearLayout

    private lateinit var server: String
    private lateinit var password: String
    private lateinit var username: String
    private lateinit var companies: ArrayList<String>

    private lateinit var apiViewModel: ApiViewModel
    private lateinit var influxGraphs: InfluxGraphs

    private val fromCalendar = Calendar.getInstance()
    private val toCalendar = Calendar.getInstance()

    enum class ViewSwitch {
        CHOOSER,
        CREATOR,
        PLOT

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val policy = ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val createPlotButton = binding.createPlotButton
        val deletePlotButton = binding.deletePlotButton
        val savePlotButton = binding.saveGraphButton
        val showPlotButton = binding.showPlotButton
        val retButton = binding.graphCloseButton
        graphSpinner = binding.graphChooser
        addView = binding.graphCreatorView
        chooserView = binding.grapChooserView
        graphView = binding.graphLayout

        val b = getIntent().getExtras()
        if (b != null) {
            server = b.getString("address").toString()
            username = b.getString("username").toString()
            password = b.getString("password").toString()
            companies = b.getStringArrayList("companies") as ArrayList<String>
        }

        apiViewModel =
            ViewModelProvider(this, ApiViewModelFactory()).get(ApiViewModel::class.java)

        apiViewModel.apiFormState.observe(this@MainActivity, Observer {
            val apiState = it ?: return@Observer

            if (!apiState.isDataValid) {
                Toast.makeText(applicationContext, "Received invalid data!", Toast.LENGTH_SHORT)
                    .show()
            }
        })

        apiViewModel.apiResult.observe(this@MainActivity, Observer {
            val queryResult = it ?: return@Observer

            if (queryResult.error != null) {
                Toast.makeText(
                    applicationContext,
                    "Databázový dotaz se nepovedl!",
                    Toast.LENGTH_SHORT
                ).show()
            }
            if (queryResult.success != null) {
                updateGraphCreator(queryResult.success)
            }
        })

        switchView(ViewSwitch.CHOOSER)

        createPlotButton.setOnClickListener {
            switchView(ViewSwitch.CREATOR)
            resetGraphAdder()
            apiViewModel.getBuckets(server, username, password)
        }

        savePlotButton.setOnClickListener {
            switchView(ViewSwitch.CHOOSER)
            saveGraph();
        }

        showPlotButton.setOnClickListener {
            plotGraph()
            switchView(ViewSwitch.PLOT)
        }

        deletePlotButton.setOnClickListener {
            deleteGraph()
        }

        retButton.setOnClickListener {
            switchView(ViewSwitch.CHOOSER)
        }

        setUpGraphAdder()
        setupTimePickers()
        loadSavedGraphs()

    }

    private fun plotGraph() {
        val actualGraph = binding.graphChooser.selectedItem.toString()
        for (graphInfo in influxGraphs.graphs) {
            if (graphInfo.graphName == actualGraph) {
                apiViewModel.getGraphData(
                    serverAddress = server,
                    organization = username,
                    token = password,
                    bucket = graphInfo.bucket,
                    location = graphInfo.place,
                    device = graphInfo.device,
                    measurements = graphInfo.measurements,
                    timeFrom = graphInfo.timeFrom,
                    timeTo = graphInfo.timeTo
                )
                return
            }
        }
    }


    private fun loadSavedGraphs() {
        val mPrefs = getPreferences(MODE_PRIVATE)
        val gson = Gson()
        val json = mPrefs.getString("SavedGraphs", "")
        if (json != "") {
            influxGraphs = gson.fromJson(json, InfluxGraphs::class.java)
        } else {
            influxGraphs = InfluxGraphs(graphs = ArrayList())
        }
        updateSavedGraphsDropdown()
    }

    private fun saveGraph() {
        val company = binding.companySpinner.selectedItem.toString()
        val deviceType = binding.deviceTypeSpinner.selectedItem.toString()
        //todo mapovani device type na device name
        val deviceName = binding.deviceNameSpinner.selectedItem.toString()
        val measurements = binding.checkboxLayout
        val childCount = measurements.getChildCount()
        val measurementList: ArrayList<String> = ArrayList()
        val graphName = binding.graphName.text.toString()

        for (i in 0 until childCount) {
            val v: View = measurements.getChildAt(i)
            if (v is CheckBox) {
                if (v.isChecked) {
                    measurementList.add(v.getHint().toString())
                }
            }
        }

        if (company == "" || deviceType == "" || deviceName == " " || measurementList.isEmpty() || graphName == "") {

            Toast.makeText(
                applicationContext,
                "Nebyly vyplněny všechny parametry!",
                Toast.LENGTH_SHORT
            )
                .show()
            return
        }


        val graphInfo = GraphInfo(
            bucket = company,
            place = deviceType,
            device = deviceName,
            measurements = measurementList,
            timeFrom = fromCalendar,
            timeTo = toCalendar,
            graphName = graphName
        )

        for (graph in influxGraphs.graphs) {
            if (graph.graphName == graphInfo.graphName) {
                Toast.makeText(
                    applicationContext,
                    "Graf se stejným názvem již existuje a nebyl vytvořen nový!",
                    Toast.LENGTH_SHORT
                )
                    .show()
                return
            }
        }


        influxGraphs.graphs.add(graphInfo)

        updateSavedGraphsDropdown()
    }

    private fun updateSavedGraphsDropdown() {
        val graphDropdown = binding.graphChooser

        val graphs = ArrayList<String>()
        for (graph in influxGraphs.graphs) {
            graphs.add(graph.graphName)
        }
        setValuesInSpinner(graphs, graphDropdown, "Žádné uložené grafy")

        val mPrefs = getPreferences(MODE_PRIVATE)
        val prefsEditor: SharedPreferences.Editor = mPrefs.edit()
        val gson = Gson()
        val json = gson.toJson(influxGraphs)
        prefsEditor.putString("SavedGraphs", json)
        prefsEditor.apply()
    }

    private fun deleteGraph() {
        val actualGraph = binding.graphChooser.selectedItem.toString()
        for (graph in influxGraphs.graphs) {
            if (graph.graphName == actualGraph) {
                influxGraphs.graphs.remove(graph)
                break
            }
        }

        updateSavedGraphsDropdown()
    }

    private fun resetGraphAdder() {
        val companySpinner = binding.companySpinner
        val deviceTypeSpinner = binding.deviceTypeSpinner
        val deviceNameSpinner = binding.deviceNameSpinner
        val name = binding.graphName

        name.setText("")

        setValuesInSpinner(companies, companySpinner, "Nebyla nalezena žádná společnost!")
        setValuesInSpinner(ArrayList(), deviceTypeSpinner, "Nebylo nalezeno žádné zařízení!")
        setValuesInSpinner(ArrayList(), deviceNameSpinner, "Nebylo nalezeno žádné zařízení!")

    }

    private fun setUpGraphAdder() {
        val companySpinner = binding.companySpinner

        companySpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {
                //nothing
            }

            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                val company = companySpinner.selectedItem.toString()
                apiViewModel.getLocation(server, username, password, company)
            }

        }

        val deviceTypeSpinner = binding.deviceTypeSpinner
        deviceTypeSpinner.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
            override fun onNothingSelected(parent: AdapterView<*>?) {
                //nothing
            }

            override fun onItemSelected(
                parent: AdapterView<*>?,
                view: View?,
                position: Int,
                id: Long
            ) {
                val location = deviceTypeSpinner.selectedItem.toString()
                val bucket = companySpinner.selectedItem.toString()
                apiViewModel.getDevice(server, username, password, bucket, location)
            }

        }

        val deviceNameSpinner = binding.deviceNameSpinner
        binding.deviceNameSpinner.onItemSelectedListener =
            object : AdapterView.OnItemSelectedListener {
                override fun onNothingSelected(parent: AdapterView<*>?) {
                    //nothing
                }

                override fun onItemSelected(
                    parent: AdapterView<*>?,
                    view: View?,
                    position: Int,
                    id: Long
                ) {
                    val company = companySpinner.selectedItem.toString()
                    val deviceType = deviceTypeSpinner.selectedItem.toString()
                    val deviceId = deviceNameSpinner.selectedItem.toString()
                    apiViewModel.getMeasurements(
                        server,
                        username,
                        password,
                        company,
                        deviceType,
                        deviceId
                    )
                }
            }
    }

    private fun updateGraphCreator(data: ApiDataView) {
        if (data.locationList != null) {
            val locationSpinner = binding.deviceTypeSpinner
            setValuesInSpinner(data.locationList, locationSpinner, "Nebylo nalezeno žádné místo")
        }
        if (data.deviceList != null) {
            val deviceSpinner = binding.deviceNameSpinner
            setValuesInSpinner(data.deviceList, deviceSpinner, "Nebylo nalezeno žádné zařízení")
        }
        if (data.measurementList != null) {
            val measurementLayout = findViewById<LinearLayout>(R.id.checkbox_layout)

            measurementLayout.removeAllViews()

            for (measurement in data.measurementList) {
                val checkBox = CheckBox(this)
                checkBox.setHint(measurement)
                val params = LinearLayout.LayoutParams(
                    ViewGroup.LayoutParams.WRAP_CONTENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT,
                )
                params.gravity = Gravity.CENTER
                checkBox.layoutParams = params
                checkBox.setTextColor(Color.BLACK)
                checkBox.setHintTextColor(Color.BLACK)
                measurementLayout.addView(checkBox)
            }
            //todo loading screen?
        }
        if (data.dataList != null) {
            //todo move
            if (data.dataList.isEmpty()) return

            val graph = findViewById<View>(R.id.graph) as GraphView
            val rnd = Random()
            graph.removeAllSeries()

            val actualGraph = binding.graphChooser.selectedItem.toString()
            var measurementList = ArrayList<String>()
            for (graphInfo in influxGraphs.graphs) {
                if (graphInfo.graphName == actualGraph) {
                    measurementList = graphInfo.measurements
                }
            }


            for (measurement in measurementList) {
                val series: LineGraphSeries<DataPoint> = LineGraphSeries()
                for (value in data.dataList) {
                    if (value.measurement == measurement) {
                        series.appendData(DataPoint(Date.from(value.time), value.value), true, 1000)
                        series.color =
                            Color.argb(255, rnd.nextInt(256), rnd.nextInt(256), rnd.nextInt(256))
                        series.isDrawDataPoints = true
                        series.title = measurement
                    }
                }
                graph.addSeries(series)
            }
            val timeFormat = SimpleDateFormat("HH:mm");
            graph.getGridLabelRenderer()
                .setLabelFormatter(DateAsXAxisLabelFormatter(graph.getContext(), timeFormat));
            graph.getLegendRenderer().setVisible(true);
            graph.getLegendRenderer().setAlign(LegendRenderer.LegendAlign.TOP)
        }
    }

    private fun switchView(viewSwitch: ViewSwitch) {
        when (viewSwitch) {
            ViewSwitch.CHOOSER -> {
                chooserView.visibility = View.VISIBLE
                addView.visibility = View.GONE
                graphView.visibility = View.GONE
            }

            ViewSwitch.CREATOR -> {
                addView.visibility = View.VISIBLE
                chooserView.visibility = View.GONE
                graphView.visibility = View.GONE
            }

            ViewSwitch.PLOT -> {
                graphView.visibility = View.VISIBLE
                addView.visibility = View.GONE
                chooserView.visibility = View.GONE
            }
        }
    }

    private fun setValuesInSpinner(list: ArrayList<String>, spinner: Spinner, emptyText: String) {
        if (list.isEmpty()) {
            list.add(emptyText)
            spinner.setEnabled(false)
        } else {
            spinner.setEnabled(true)
        }

        val arrayAdapter =
            ArrayAdapter(this, R.layout.support_simple_spinner_dropdown_item, list)
        arrayAdapter.setDropDownViewResource(R.layout.support_simple_spinner_dropdown_item)
        spinner.adapter = arrayAdapter
    }

    private fun setupTimePickers() {
        val dateFormat = SimpleDateFormat("MM/dd/yyyy");
        val timeFormat = SimpleDateFormat("HH:mm");
        fromCalendar.add(Calendar.DAY_OF_MONTH, -1)

        val fromDate = findViewById<EditText>(R.id.date_from_edit_text)
        fromDate.transformIntoDatePicker(this, "dd/MM/yyyy", fromCalendar)
        fromDate.setText(dateFormat.format(fromCalendar.getTime()))

        val toDate = findViewById<EditText>(R.id.date_to_edit_text)
        toDate.transformIntoDatePicker(this, "dd/MM/yyyy", toCalendar)
        toDate.setText(dateFormat.format(toCalendar.getTime()))

        val fromTime = findViewById<EditText>(R.id.time_from_edit_text)
        fromTime.transformIntoTimePicker(this, "HH:mm", fromCalendar)
        fromTime.setText(timeFormat.format(fromCalendar.getTime()))

        val toTime = findViewById<EditText>(R.id.time_to_edit_text)
        toTime.transformIntoTimePicker(this, "HH:mm", toCalendar)
        toTime.setText(timeFormat.format(toCalendar.getTime()))
    }

    fun EditText.transformIntoDatePicker(
        context: Context,
        format: String,
        calendar: Calendar,
        maxDate: Date? = null
    ) {
        isFocusableInTouchMode = false
        isClickable = true
        isFocusable = false


        val datePickerOnDataSetListener =
            DatePickerDialog.OnDateSetListener { _, year, monthOfYear, dayOfMonth ->
                calendar.set(Calendar.YEAR, year)
                calendar.set(Calendar.MONTH, monthOfYear)
                calendar.set(Calendar.DAY_OF_MONTH, dayOfMonth)
                val sdf = SimpleDateFormat(format, Locale.UK)
                setText(sdf.format(calendar.time))
            }


        setOnClickListener {
            DatePickerDialog(
                context, datePickerOnDataSetListener, calendar
                    .get(Calendar.YEAR), calendar.get(Calendar.MONTH),
                calendar.get(Calendar.DAY_OF_MONTH)
            ).run {
                maxDate?.time?.also { datePicker.maxDate = it }
                show()
            }
        }
    }

    fun EditText.transformIntoTimePicker(
        context: Context,
        format: String,
        calendar: Calendar,
        maxDate: Date? = null
    ) {
        isFocusableInTouchMode = false
        isClickable = true
        isFocusable = false


        val timeSetListener = TimePickerDialog.OnTimeSetListener { timePicker, hour, minute ->
            calendar.set(Calendar.HOUR_OF_DAY, hour)
            calendar.set(Calendar.MINUTE, minute)
            setText(SimpleDateFormat(format).format(calendar.time))
        }

        setOnClickListener {
            TimePickerDialog(
                context,
                timeSetListener,
                calendar.get(Calendar.HOUR_OF_DAY),
                calendar.get(Calendar.MINUTE),
                true
            ).show()
        }

    }
}