package com.emu.slimnow.prefer

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.preference.Preference
import androidx.preference.PreferenceFragmentCompat
import com.emu.slimnow.R

import com.emu.slimnow.databinding.ActivitySettingsBinding

class SettingsActivity : AppCompatActivity(), PreferenceFragmentCompat.OnPreferenceDisplayDialogCallback {
    private val binding by lazy { ActivitySettingsBinding.inflate(layoutInflater) }

    // At this moment, we don't have any other fragment different from the GlobalFragment
    private val selectedFragment by lazy { GlobalFragment() }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)

        // Enables action bar and the "return button" inside of our fragment view
        WindowCompat.setDecorFitsSystemWindows(window, false)

        setSupportActionBar(binding.settingsToolbar)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        supportFragmentManager.beginTransaction()
            .replace(R.id.fragmentSettings, selectedFragment)
            .commit()
    }

    override fun onPreferenceDisplayDialog(
        caller: PreferenceFragmentCompat,
        pref: Preference
    ): Boolean {
        TODO("Not yet implemented")

    }

}
