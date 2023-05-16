
#include "COP0Super.h"

#pragma once

// Reset System Result Code
namespace Slim {
    enum ResetState {
        RsrFullyReset = 10
    };
}

namespace Slim::ee {
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
