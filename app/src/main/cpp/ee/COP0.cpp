// This source code is part of SlimNow project

#include <cstring>

#include <ee/COP0_super.h>

namespace slim::ee {

    CoreR5900::CoreR5900() {
        // Cleaning all registers states, there's no need to doing this, but it's good for maintainer
        // our emulation state clean and responsible!
        m_GPR = new u32[mipsGprCount];
        m_linearRegisters = new u128[mipsScalarCount];

        std::memset(m_GPR, 0, sizeof(u32) * mipsGprCount);
        std::memset(m_linearRegisters, 0, sizeof(u128) * mipsScalarCount);

    }

    CoreR5900::~CoreR5900() {
        // Deleting our registers banks
        delete[] m_GPR;
        delete[] m_linearRegisters;
    }
}
