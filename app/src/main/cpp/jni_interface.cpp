// This source code is part of SlimNow project
#include <jni.h>

#include <slim_vm.h>
#include <tasks/executor.h>
#include <logger.h>

namespace slim::Unordered {
    [[maybe_unused]] auto psxVMExist{false};

    jobject createPSXVM([[maybe_unused]] JavaVM* vm) {
        pthread_setname_np(pthread_self(), "Spawn PSX-VM");
        Logger::putsInfo("Spawning PS2 Virtual Machine");

        g_psxMachine = std::make_unique<VirtualMachine>();
        psxVMExist = g_psxMachine != nullptr;

        return {};
    }
}

// Called from JNI native libraries loader, after load our library!
// We can initialize everything here before the application ends up it's Splash Screen
__attribute__((visibility("default"))) extern "C" jint
JNI_OnLoad([[maybe_unused]] JavaVM* vm, [[maybe_unused]] void* reserved) {
    using namespace slim;
    const auto jniVersion{JNI_VERSION_1_6};

    // Creating our emulator's virtual machine instance while the app is loading!
    tasks::g_taskSolver = std::make_unique<tasks::UnorderedExecutor>(vm);
    tasks::g_taskSolver->pushTask(Unordered::createPSXVM);

    return jniVersion;
}

