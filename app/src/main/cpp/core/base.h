// This source code is part of SlimNow project
#pragma once

#include <cstdint>

namespace slim {
    using u8 = std::uint8_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

#pragma pack(push, 16)
    union u128 {
        [[maybe_unused]] u8 asBytes[16];
        [[maybe_unused]] u32 asDWords[4];
        [[maybe_unused]] u64 asQWords[2];
    };
#pragma pack(pop)

}