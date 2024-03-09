package com.trubka.iot_plot.api_connection

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.trubka.iot_plot.R
import java.util.*
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class ApiViewModel(private val apiRepository: ApiRepository) : ViewModel() {
    private val _apiForm = MutableLiveData<ApiFormState>()
    val apiFormState: LiveData<ApiFormState> = _apiForm

    private val _apiResult = MutableLiveData<ApiResult>()
    val apiResult: LiveData<ApiResult> = _apiResult

    private val executor: ExecutorService = Executors.newCachedThreadPool()

    fun login(serverAddress: String, username: String, password: String) {
        executor.execute{
            try{
                if(apiRepository.makeLoginRequest(serverAddress, password, username)){
                    _apiResult.postValue(ApiResult(success = ApiDataView(apiAddress = serverAddress, username = username, password = password)))
                }else{
                    _apiResult.postValue(ApiResult(error = R.string.login_failed))
                }
            }catch (e: java.lang.Exception){
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getBuckets(serverAddress: String, organization: String, token: String){
        executor.execute{
            try{
                val result = apiRepository.makeBucketListRequest(serverAddress, token, organization)
                _apiResult.postValue(ApiResult(success = ApiDataView(bucketList = result)))
            }catch (e: java.lang.Exception){
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getLocation(serverAddress: String, organization: String, token: String, bucket: String){
        executor.execute{
            try{
                val result = apiRepository.makeLocationListRequest(serverAddress, token, organization, bucket)
                _apiResult.postValue(ApiResult(success = ApiDataView(locationList = result)))
            }catch (e: java.lang.Exception){
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getDevice(serverAddress: String, organization: String, token: String, bucket: String, location: String){
        executor.execute{
            try{
                val result = apiRepository.makeDeviceListRequest(serverAddress, token, organization, bucket, location)
                _apiResult.postValue(ApiResult(success = ApiDataView(deviceList = result)))
            }catch (e: java.lang.Exception){
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getMeasurements(serverAddress: String, organization: String, token: String, bucket: String, location: String, device: String){
        executor.execute{
            try{
                val result = apiRepository.makeMeasurementListRequest(serverAddress, token, organization, bucket, location, device)
                _apiResult.postValue(ApiResult(success = ApiDataView(measurementList = result)))
            }catch (e: java.lang.Exception){
                _apiResult.postValue(ApiResult(error = R.string.login_failed))
            }
        }
    }

    fun getGraphData(serverAddress: String, organization: String, token: String, bucket: String, location: String, device: String,
                     measurements: List<String>, timeFrom: Calendar, timeTo: Calendar
    ){
        executor.execute{
            try{
                val result = apiRepository.makeGraphRequest(serverAddress, token, organization, bucket, location, device, measurements, timeFrom, timeTo)
                _apiResult.postValue(ApiResult(success = ApiDataView(dataList = result)))
            }catch (e: java.lang.Exception){
                _apiResult.postValue(ApiResult(error = R.string.data_load_failed))
            }
        }
    }

    fun loginDataChanged(serverAddress: String, username: String, password: String) {
        if (!isAddressValid(serverAddress)) {
            _apiForm.value = ApiFormState(addressError = R.string.invalid_address)
        } else if (!isUsernameValid(username)) {
            _apiForm.value = ApiFormState(usernameError = R.string.invalid_username)
        } else if(!isPasswordValid(password)){
            _apiForm.value = ApiFormState(passwordError = R.string.invalid_password)
        }else{
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