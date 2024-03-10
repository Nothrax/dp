package com.trubka.iot_plot.api_connection

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.trubka.iot_plot.R
import java.util.*
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import kotlin.collections.ArrayList

class ApiViewModel(private val apiRepository: ApiRepository) : ViewModel() {
    private val _apiForm = MutableLiveData<ApiFormState>()
    val apiFormState: LiveData<ApiFormState> = _apiForm

    private val _apiResult = MutableLiveData<ApiResult>()
    val apiResult: LiveData<ApiResult> = _apiResult

    private val executor: ExecutorService = Executors.newCachedThreadPool()

    fun login(serverAddress: String, username: String, password: String) {
        executor.execute {
            try {
                if (apiRepository.makeLoginRequest(serverAddress, password, username)) {
                    _apiResult.postValue(
                        ApiResult(
                            success = ApiDataView(
                                apiAddress = serverAddress,
                                username = username,
                                password = password
                            )
                        )
                    )
                } else {
                    _apiResult.postValue(ApiResult(error = R.string.login_failed))
                }
            } catch (e: java.lang.Exception) {
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getCompanies(serverAddress: String, username: String, password: String) {
        executor.execute {
            try {
                val result = apiRepository.getCompanies(
                    address = serverAddress,
                    password = password,
                    username = username
                )
                _apiResult.postValue(ApiResult(success = ApiDataView(companyList = result)))
            } catch (e: java.lang.Exception) {
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getDevices(server: String, user: String, password: String, company: String) {
        executor.execute {
            try {
                val result: ArrayList<Device> =
                    apiRepository.makeDeviceListRequest(server, user, password, company)
                _apiResult.postValue(ApiResult(success = ApiDataView(deviceList = result)))
            } catch (e: java.lang.Exception) {
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun setDeviceType(deviceType: DeviceType) {
        _apiResult.postValue(ApiResult(success = ApiDataView(currentDeviceType = deviceType)))
    }

    fun getGraphData(
        address: String,
        username: String,
        password: String,
        company: String,
        deviceType: DeviceType,
        deviceId: Int,
        fields: List<String>,
        timeFrom: Calendar,
        timeTo: Calendar
    ) {
        executor.execute {
            try {
                val result = apiRepository.makeGraphRequest(
                    address = address,
                    username = username,
                    password = password,
                    company = company,
                    deviceType = deviceType,
                    deviceId = deviceId,
                    fields = fields,
                    timeFrom = timeFrom,
                    timeTo = timeTo
                )
                _apiResult.postValue(ApiResult(success = ApiDataView(measurements = result)))
            } catch (e: java.lang.Exception) {
                _apiResult.postValue(ApiResult(error = R.string.data_load_failed))
            }
        }
    }

    fun loginDataChanged(serverAddress: String, username: String, password: String) {
        if (!isAddressValid(serverAddress)) {
            _apiForm.value = ApiFormState(addressError = R.string.invalid_address)
        } else if (!isUsernameValid(username)) {
            _apiForm.value = ApiFormState(usernameError = R.string.invalid_username)
        } else if (!isPasswordValid(password)) {
            _apiForm.value = ApiFormState(passwordError = R.string.invalid_password)
        } else {
            _apiForm.value = ApiFormState(isDataValid = true)
        }
    }

    private fun isAddressValid(address: String): Boolean {
        return address.isNotBlank()
    }

    private fun isUsernameValid(organization: String): Boolean {
        return organization.isNotBlank()
    }

    private fun isPasswordValid(token: String): Boolean {
        return token.length > 1
    }
}