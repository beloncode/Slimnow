// This source code is part of SlimNow project
#include <thread>
#include <jni.h>

#include "SlimVM.h"

namespace Slim::Thread {
    thread_local pthread_key_t t_pthreadKey;
    thread_local auto t_hasAttached{false};

    void onNativeExit(void* javaVM) {
        auto vm{reinterpret_cast<JavaVM*>(javaVM)};
        if (t_hasAttached) {
            vm->DetachCurrentThread();
        }

        // If we reach here, we already have a valid pthread key!
        pthread_key_delete(t_pthreadKey);
    }

    auto attachToJavaVM(JavaVM* vm, const std::string& thName) {
        JNIEnv* local{};
        pthread_setname_np(pthread_self(), thName.c_str());

        if (vm->GetEnv(reinterpret_cast<void**>(&local),
                       JNI_VERSION_1_6) == JNI_EDETACHED) {
            vm->AttachCurrentThread(&local, nullptr);
            t_hasAttached = true;
        }

        pthread_key_create(&t_pthreadKey, onNativeExit);
        pthread_setspecific(t_pthreadKey, vm);

        return local;
    }
}

namespace Slim::Unordered {
    [[maybe_unused]] auto psxVMExist{false};

    void createPSXVM(JavaVM* vm) {
        Thread::attachToJavaVM(vm, "Spawn PSX-VM");

        g_psxMachine = std::make_unique<VirtualMachine>();
        psxVMExist = g_psxMachine != nullptr;

    }
}

// Called from JNI native libraries loader, after load our library!
// We can initialize everything here before the application ends up it's Splash Screen
__attribute__((visibility("default"))) extern "C" jint
JNI_OnLoad([[maybe_unused]] JavaVM* vm, [[maybe_unused]] void* reserved) {
    const auto jniVersion{JNI_VERSION_1_6};

    // Creating our emulator's virtual machine instance while the app is loading!
    std::thread startThread(Slim::Unordered::createPSXVM, vm);
    startThread.join();

    return jniVersion;
}

