package com.emu.slimnow.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.emu.slimnow.R

class SaveManagerFragment : Fragment() {
    companion object {
        fun getInstance() = SaveManagerFragment()
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_savemanager, container, false)
    }
}