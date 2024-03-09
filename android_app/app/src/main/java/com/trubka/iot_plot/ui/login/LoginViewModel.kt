package com.trubka.iot_plot.ui.login

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.trubka.iot_plot.data.LoginRepository
import com.trubka.iot_plot.data.Result

import com.trubka.iot_plot.R

import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors


class LoginViewModel(private val loginRepository: LoginRepository) : ViewModel() {

    private val _loginForm = MutableLiveData<LoginFormState>()
    val loginFormState: LiveData<LoginFormState> = _loginForm

    private val _loginResult = MutableLiveData<LoginResult>()
    val loginResult: LiveData<LoginResult> = _loginResult

    private val executor: ExecutorService = Executors.newSingleThreadExecutor()

    fun login(serverAddress: String, username: String, password: String) {
        executor.execute{
            val result = loginRepository.login(serverAddress, username, password)
            if (result is Result.Success) {
                _loginResult.postValue(LoginResult(success = LoggedInUserView(apiAddress = serverAddress, username = username, password = password, companies = result.data.companies)))
            } else {
                _loginResult.postValue(LoginResult(error = R.string.login_failed))
            }
        }
    }

    fun loginDataChanged(serverAddress: String, organization: String, token: String) {
        if (!isAddressValid(serverAddress)) {
            _loginForm.value = LoginFormState(addressError = R.string.invalid_address)
        } else if (!isUsernameValid(organization)) {
            _loginForm.value = LoginFormState(usernameError = R.string.invalid_username)
        } else if(!isPasswordValid(token)){
            _loginForm.value = LoginFormState(passwordError = R.string.invalid_password)
        }else{
            _loginForm.value = LoginFormState(isDataValid = true)
        }
    }

    private fun isAddressValid(address: String): Boolean {
        return address.isNotBlank()
    }

    private fun isUsernameValid(organization: String): Boolean {
        return organization.isNotBlank()
    }

    private fun isPasswordValid(password: String): Boolean {
        return password.length > 1
    }
}