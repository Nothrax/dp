package com.trubka.iot_plot.api_connection

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider

class ApiViewModelFactory : ViewModelProvider.Factory {

    @Suppress("UNCHECKED_CAST")
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(ApiViewModel::class.java)) {
            return ApiViewModel(
                apiRepository = ApiRepository()
            ) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}