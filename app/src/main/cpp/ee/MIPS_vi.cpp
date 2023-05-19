// This source code is part of SlimNow project
#include <ee/ee_soc.h>

namespace slim::ee {

    CentralProcessor::CentralProcessor() :
        m_COP0() {
        m_COP0 = new CoreR5900();
    }

    CentralProcessor::~CentralProcessor() {
        delete[] m_COP0;
    }

    // Doing a reset in our CPU to it's initial state, the JIT must be configured accord
    ResetState CentralProcessor::resetSystem() {
        return ResetState::RsrFullyReset;
    }

}