package com.emu.slimnow.prefer

import android.os.Bundle
import androidx.preference.PreferenceFragmentCompat
import com.emu.slimnow.R

class GlobalFragment: PreferenceFragmentCompat() {
    // This constructor will load all available preferences that could be within or Global
    // Preferences Activity (Settings Activity)
    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        addPreferencesFromResource(R.xml.prefer_app)
    }

}