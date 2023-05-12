package com.emu.slimnow.model

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.asLiveData
import androidx.lifecycle.viewModelScope
import com.emu.slimnow.UserPrefer
import com.emu.slimnow.prefer.SettingsSerializer.userPrefer
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.launch

class UserPreferModel(application: Application) : AndroidViewModel(application) {

    private val context by lazy { application.applicationContext }

    private val preferReadable: Flow<UserPrefer> = context.userPrefer.data
    val preferStorage = preferReadable.asLiveData()

    fun setupIsFinished(isFinished: Boolean) = viewModelScope.launch(Dispatchers.IO) {
        context.userPrefer.updateData { user ->
            user.toBuilder()
                .setHasConfigured(isFinished)
                .build()
        }
    }

}