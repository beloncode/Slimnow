#pragma once

#include <cstdint>
#include <arm_neon.h>

namespace Slim::ee {

    using register32 = std::int32_t;
    using linear128 = int8x8x2_t;
    static_assert(sizeof(linear128) == 16, "linear128 must be a 128 bit wide type");

    class CoreR5900 {
        // Count of registers inside of our MIPS
        static constexpr auto MIPS_GPR_COUNT{32};
        static constexpr auto MIPS_SCALAR_COUNT{32};

    public:
        CoreR5900();
        ~CoreR5900();

    private:
        [[maybe_unused]] register32* m_GPR;
        [[maybe_unused]] linear128* m_linearRegisters;

    };
}
