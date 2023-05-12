package com.emu.slimnow

import android.os.Bundle
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen

import com.emu.slimnow.databinding.ActivitySetupBinding
import com.emu.slimnow.model.SetupModel
import com.emu.slimnow.model.UserPreferModel

class SetupActivity: AppCompatActivity() {
    private val binding by lazy { ActivitySetupBinding.inflate(layoutInflater) }
    private val setupModel: SetupModel by viewModels()
    private val prefers: UserPreferModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        installSplashScreen().apply {
            setKeepOnScreenCondition {
                setupModel.isLoading.value
            }
        }
        setContentView(binding.root)
        binding.setupNext.apply {
            setOnClickListener { finish() }
        }
    }

    override fun finish() {
        super.finish()
        setResult(RESULT_OK)
        prefers.setupIsFinished(true)
    }

}