
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
    class EmotionR5900 {

    public:
        EmotionR5900() = default;
        static ResetState resetCPU();

    };
}
