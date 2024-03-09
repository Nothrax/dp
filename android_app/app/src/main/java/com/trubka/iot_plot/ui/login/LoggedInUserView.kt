package com.trubka.iot_plot.ui.login

import java.io.Serializable

/**
 * User details post authentication that is exposed to the UI
 */
data class LoggedInUserView(
    val apiAddress: String,
    val username: String,
    val password: String,
    val companies: ArrayList<String>
)