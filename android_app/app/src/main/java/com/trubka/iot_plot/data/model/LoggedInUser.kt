package com.trubka.iot_plot.data.model

/**
 * Data class that captures user information for logged in users retrieved from LoginRepository
 */
data class LoggedInUser(
    val username: String,
    val password: String
)