// This source code is part of SlimNow project

#pragma once

#include <array>

#include <android/log.h>
#include <fmt/format.h>

#include <core/base.h>

namespace slim {

    enum MessageSeverity {
        MsgSeveritySuccess [[maybe_unused]] = ANDROID_LOG_DEFAULT,
        MsgSeverityFailed  [[maybe_unused]] = ANDROID_LOG_ERROR,
        MsgSeverityInfo    [[maybe_unused]] = ANDROID_LOG_INFO,
        MsgSeverityVerbose [[maybe_unused]] = ANDROID_LOG_VERBOSE
    };

    static constexpr u8 messageBufferSz{0x44};
    class Logger {
        static constexpr std::string_view tagCard{"SlimNative"};

    public:
        template <typename ...T>
        static auto writeMessageBuffer(MessageSeverity sevMsg,
                                       fmt::format_string<T...> format, T &&... args) {
            auto mimicFile{fmemopen(m_uniqueBuffer.data(), m_uniqueBuffer.size(), "w")};
            fmt::print(mimicFile, format, args...);
            fclose(mimicFile);

            return __android_log_write(sevMsg, tagCard.data(), m_uniqueBuffer.data());
        }

        template <typename ...T>
        static auto writeInfo(fmt::format_string<T...> format, T &&... args) {
            return writeMessageBuffer(MessageSeverity::MsgSeverityInfo, format, args...);
        }

        static auto putsMessage(MessageSeverity sevMsg,
                                const std::string_view& outMessage) {
            return __android_log_write(sevMsg, tagCard.data(), outMessage.data());
        }

        static auto putsInfo(const std::string_view& outMessage) {
            putsMessage(MessageSeverity::MsgSeverityInfo, outMessage);
        }

    private:
        static thread_local std::array<char, messageBufferSz> m_uniqueBuffer;
    };
}

