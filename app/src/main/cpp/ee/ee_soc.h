// This source code is part of SlimNow project
#include <ee/COP0_super.h>

#pragma once

// Reset System Result Code
namespace slim {
    enum ResetState {
        RsrFullyReset = 10
    };
}

namespace slim::ee {
    // Emotion Engine consist of eight separate pieces of an elegant and dedicated hardware,
    // everything embed in one (not little :0) SOC!
    class CentralProcessor {
    public:
        CentralProcessor();
        ~CentralProcessor();
        static ResetState resetSystem();
    private:
        [[maybe_unused]] CoreR5900* m_COP0;
    };
}
