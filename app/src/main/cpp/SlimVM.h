// This source code is part of SlimNow project

#pragma once
#include <memory>
#include <cstring>

#include <ee/EESoc.h>

namespace Slim {
    class VirtualMachine {
        static constexpr auto DESIRED_FPS_BY_DEFAULT{60};

    public:
        VirtualMachine();
    private:
        [[maybe_unused]] u8 m_desiredFPS{DESIRED_FPS_BY_DEFAULT};

        // Systems being emulated by our Virtual Machine

        // A cute EE, yey, it's CUTE, for the EE (only) -> YOU can't get me down, your pile of dirt!
        std::shared_ptr<ee::CentralProcessor> m_cuteEE;
    };

    [[maybe_unused]] extern std::unique_ptr<VirtualMachine> g_psxMachine;
}

