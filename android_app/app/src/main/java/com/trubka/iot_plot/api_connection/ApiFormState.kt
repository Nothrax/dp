package com.trubka.iot_plot.api_connection

/**
 * Data validation state of the login form.
 */
data class ApiFormState (
    val addressError: Int? = null,
    val usernameError: Int? = null,
    val passwordError: Int? = null,
    val isDataValid: Boolean = false
)