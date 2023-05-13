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

data class PreferEntryData(
    var boolValue: Boolean = false,
    var textValue: String = "")

enum class PreferList {
    PREFER_ROOT_STORAGE,
    PREFER_SETUP_IS_FINISHED
}

class UserPreferModel(application: Application) : AndroidViewModel(application) {

    private val context by lazy { application.applicationContext }

    private val preferReadable: Flow<UserPrefer> = context.userPrefer.data
    val preferStorage = preferReadable.asLiveData()

    fun setPreferData(value: PreferEntryData, pl: PreferList) = viewModelScope.launch(Dispatchers.IO) {
        context.userPrefer.updateData { user ->
            val builder = user.toBuilder()
            when (pl) {
                PreferList.PREFER_ROOT_STORAGE -> builder.rootDirStorage = value.textValue
                PreferList.PREFER_SETUP_IS_FINISHED -> builder.hasConfigured = value.boolValue
            }
            builder.build()
        }
    }

}