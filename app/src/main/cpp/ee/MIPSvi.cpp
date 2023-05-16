#include "EESoc.h"


namespace Slim::ee {

    // Doing a reset in our CPU to it's initial state, the JIT must be configured accord
    ResetState EmotionR5900::resetCPU() {
        return ResetState::RsrFullyReset;
    }

}