package com.emu.slimnow

import android.content.Intent
import android.os.Bundle
import android.view.MenuItem
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.core.view.WindowCompat
import androidx.fragment.app.Fragment
import com.emu.slimnow.databinding.ActivityMainBinding
import com.emu.slimnow.model.MainActivityModel
import com.emu.slimnow.prefer.SettingsActivity
import com.emu.slimnow.ui.*

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private fun replaceFragmentView(menuNaviItem: MenuItem) {
        val layoutFragment: Any = when (menuNaviItem.itemId) {
            R.id.romSection -> RomSelectionFragment.getInstance()
            R.id.saveManager -> SaveManagerFragment.getInstance()
            R.id.gallery -> GalleryFragment.getInstance()
            R.id.logMessages -> LogFragment.getInstance()
            else -> { }
        }

        layoutFragment.let {
            supportFragmentManager.beginTransaction()
                .replace(R.id.fragmentFactory, it as Fragment)
                .commit()
        }

    }

    private fun changeActivity(menuTopItem: MenuItem) {
        val selectedActivity: Any = when (menuTopItem.itemId) {
            R.id.settings -> { SettingsActivity::class.java }
            else -> { }
        }

        val activityIntent = Intent(this, selectedActivity as Class<*>)
        activityIntent.apply { startActivity(activityIntent) }
    }

    private val mainViewModel: MainActivityModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        installSplashScreen().apply {
            setKeepOnScreenCondition {
                mainViewModel.isLoading.value
            }
            //setOnExitAnimationListener {}
        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        WindowCompat.setDecorFitsSystemWindows(window, false)

        // Settings the first element item of our navigation bar as the Main Activity home fragment
        val firstNaviElement = binding.navi.menu.getItem(0)
        replaceFragmentView(firstNaviElement)

        binding.navi.setOnItemSelectedListener { selected ->
            replaceFragmentView(selected)
            true
        }

        binding.mainToolbar.apply {
            setOnMenuItemClickListener { buttonSelected ->
                changeActivity(buttonSelected)
                true
            }
        }

    }

    companion object {
        init {
            System.loadLibrary("slim")
        }
    }
}