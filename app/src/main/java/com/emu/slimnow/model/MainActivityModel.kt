package com.emu.slimnow.model

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch

class MainActivityModel : ViewModel() {
    private val _loadingApp = MutableStateFlow(true)
    val isLoading = _loadingApp.asStateFlow()

    init {
        viewModelScope.launch {
            // We can load all internal components before the main activity being rendering here
            // Like starts the emulator, load all library entries points, start some profile tools
            // like Perfetto and others
            // delay(2_000)
            // We're confirming that we can go to main screen now!
            _loadingApp.value = false
        }
    }


}