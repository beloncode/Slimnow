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
        MsgSeverityInfo    = ANDROID_LOG_INFO,
        MsgSeverityVerbose [[maybe_unused]] = ANDROID_LOG_VERBOSE
    };

    static constexpr u8 messageBufferSz{0x44};
    class Logger {
        static constexpr std::string_view tagCard{"SlimCore"};

    public:
        template <typename ...T>
        static auto printFormat(MessageSeverity sevMsg,
                                fmt::format_string<T...> format, T &&... args) {
            auto mimicFile{fmemopen(m_msgBuffer.data(), m_msgBuffer.size(), "w")};
            fmt::print(mimicFile, format, args...);
            fclose(mimicFile);

            return __android_log_write(sevMsg, tagCard.data(), m_msgBuffer.data());
        }

        template <typename ...T>
        static auto fmtInfo(fmt::format_string<T...> format, T &&... args) {
            return printFormat(MessageSeverity::MsgSeverityInfo, format, args...);
        }

        static auto putsMessage(MessageSeverity sevMsg,
                                const std::string_view& outMessage) {
            return __android_log_write(sevMsg, tagCard.data(), outMessage.data());
        }

        static auto putsInfo(const std::string_view& outMessage) {
            putsMessage(MessageSeverity::MsgSeverityInfo, outMessage);
        }

    private:
        static thread_local std::array<char, messageBufferSz> m_msgBuffer;
    };
}

