package com.emu.slimnow

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen

import com.emu.slimnow.databinding.ActivitySetupBinding
import com.emu.slimnow.model.PreferEntryData
import com.emu.slimnow.model.PreferList
import com.emu.slimnow.model.SetupModel
import com.emu.slimnow.model.UserPreferModel
import java.io.File

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

        assert(!binding.selectRootCheck.isChecked)

        placeRootDirectoryOption()
    }

    private val selectedDirectory = registerForActivityResult(ActivityResultContracts.
        StartActivityForResult()) { result ->
        if (result.resultCode != Activity.RESULT_OK) {
            binding.selectRootCheck.isErrorShown = true
            return@registerForActivityResult
        }

        val intentResult = result.data
        val dirResult = intentResult?.data

        prefers.setPreferData(
            value = PreferEntryData(textValue = dirResult.toString()),
            PreferList.PREFER_ROOT_STORAGE
        )

        Toast.makeText(this, "Selected directory Uri $dirResult", Toast.LENGTH_SHORT).show()

        binding.selectRootCheck.apply {
            isChecked = true
            isErrorShown = false
        }
    }

    private fun placeRootDirectoryOption() {
        prefers.preferStorage.observe(this) {
            val checkDir = File(it.rootDirStorage)
            if (checkDir.exists())
                binding.selectRootCheck.isChecked = true
        }

        binding.selectRootDirectory.setOnClickListener {
            val dirIntent = Intent(Intent.ACTION_OPEN_DOCUMENT_TREE)
            selectedDirectory.launch(dirIntent)
        }

    }

    override fun finish() {
        super.finish()
        setResult(RESULT_OK)
        prefers.setPreferData(
            value = PreferEntryData(boolValue = true),
            PreferList.PREFER_SETUP_IS_FINISHED
        )
    }

}