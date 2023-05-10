package com.emu.slimnow.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.emu.slimnow.R

class RomSelectionFragment : Fragment() {

    companion object {
        fun getInstance() = RomSelectionFragment()
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_romselection, container, false)
    }
}
