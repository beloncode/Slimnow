#pragma once

#include <core/Base.h>

namespace Slim::ee {
    static_assert(sizeof(u128) == 16, "Size of u128 must be a 128bits or/and 16bytes field value");

    class CoreR5900 {
        // Count of registers inside of our MIPS
        static constexpr auto MIPS_GPR_COUNT{32};
        static constexpr auto MIPS_SCALAR_COUNT{32};

    public:
        CoreR5900();
        ~CoreR5900();

    private:
        [[maybe_unused]] u32* m_GPR;
        [[maybe_unused]] u128* m_linearRegisters;

    };
}
