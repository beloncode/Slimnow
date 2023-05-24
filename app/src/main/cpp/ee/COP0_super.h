// This source code is part of SlimNow project
#pragma once

#include <core/base.h>

namespace slim::ee {
    static_assert(sizeof(u128) == 16, "Size of u128 must be a 128bits or/and with 16 bytes of size");

    class CoreR5900 {
        // Count of registers inside of our MIPS
        static constexpr auto mipsGprCount{32};
        static constexpr auto mipsScalarCount{32};

    public:
        CoreR5900();
        ~CoreR5900();

    private:
        u32* m_GPR;
        u128* m_linearRegisters;

    };
}
