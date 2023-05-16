// This source code is part of SlimNow project
#include "SlimVM.h"

namespace Slim {
    [[maybe_unused]] std::unique_ptr<VirtualMachine> g_psxMachine{};

    VirtualMachine::VirtualMachine() {
        m_cuteEE = std::make_shared<ee::CentralProcessor>();

        m_cuteEE->resetSystem();
    }
}



