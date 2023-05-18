#include <cstring>

#include <ee/COP0Super.h>

namespace Slim::ee {

    CoreR5900::CoreR5900() {
        // Cleaning all registers states, there's no need to doing this, but it's good for maintainer
        // our emulation state clean and responsible!
        m_GPR = new u32[MIPS_GPR_COUNT];
        m_linearRegisters = new u128[MIPS_SCALAR_COUNT];

        std::memset(m_GPR, 0, sizeof(u32) * MIPS_GPR_COUNT);
        std::memset(m_linearRegisters, 0, sizeof(u128) * MIPS_SCALAR_COUNT);

    }

    CoreR5900::~CoreR5900() {
        // Deleting our registers banks
        delete[] m_GPR;
        delete[] m_linearRegisters;
    }
}
