package com.emu.slimnow

import android.os.Bundle
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.WindowCompat
import androidx.fragment.app.Fragment
import com.emu.slimnow.databinding.ActivityMainBinding
import com.emu.slimnow.ui.*

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private fun replaceFragmentView(menuNaviItem: MenuItem) {
        var layoutFragment: Fragment? = null
        when (menuNaviItem.itemId) {
            R.id.romSection -> layoutFragment = RomSelectionFragment.getInstance()
            R.id.saveManager -> layoutFragment = SaveManagerFragment.getInstance()
            R.id.gallery -> layoutFragment = GalleryFragment.getInstance()
            R.id.logMessages -> layoutFragment = LogFragment.getInstance()
        }

        layoutFragment?.let {
            supportFragmentManager.beginTransaction()
                .replace(R.id.fragmentFactory, it)
                .commit()
        }

    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

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

    }

    companion object {
        init {
            System.loadLibrary("slim")
        }
    }
}