package com.emu.slimnow.prefer

import android.content.Context
import androidx.datastore.core.CorruptionException
import androidx.datastore.core.DataStore
import androidx.datastore.core.Serializer
import androidx.datastore.dataStore
import com.emu.slimnow.UserPrefer
import com.google.protobuf.InvalidProtocolBufferException
import java.io.InputStream
import java.io.OutputStream

object SettingsSerializer : Serializer<UserPrefer> {
    private const val USER_PREFER_FILENAME: String = "user-prefer.pb"

    override val defaultValue: UserPrefer
        get() = UserPrefer.getDefaultInstance()

    override suspend fun readFrom(input: InputStream): UserPrefer {
        try {
            return UserPrefer.parseFrom(input)
        } catch (ex: InvalidProtocolBufferException) {
            throw CorruptionException("Can't read proto", ex)
        }
    }

    override suspend fun writeTo(t: UserPrefer, output: OutputStream) = t.writeTo(output)

    val Context.userPrefer: DataStore<UserPrefer> by dataStore(
        fileName = USER_PREFER_FILENAME,
        serializer = SettingsSerializer
    )
}