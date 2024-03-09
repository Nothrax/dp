package com.trubka.iot_plot.data

import com.trubka.iot_plot.data.model.LoggedInUser
import java.io.IOException
import com.trubka.iot_plot.api_connection.ApiRepository


/**
 * Class that handles authentication w/ login credentials and retrieves user information.
 */
class LoginDataSource {

    fun login(serverAddress: String, username: String, password: String): Result<LoggedInUser> {
        val api = ApiRepository()
        return if (api.makeLoginRequest(address = serverAddress, username = username, password = password)) {
            val companies = api.getCompanies(serverAddress, username, password)
            val user = LoggedInUser(username = username, password = password, companies = companies)
            Result.Success(user)
        } else {
            Result.Error(IOException("Error logging in"))
        }
    }

    fun logout() {
        // TODO: revoke authentication
    }
}