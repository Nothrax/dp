package com.trubka.iot_plot.ui.login

/**
 * Data validation state of the login form.
 */
data class LoginFormState(
    val addressError: Int? = null,
    val usernameError: Int? = null,
    val passwordError: Int? = null,
    val isDataValid: Boolean = false
)