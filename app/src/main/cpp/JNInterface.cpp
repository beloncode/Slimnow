// This source code is part of SlimNow project
#include <jni.h>

#include "SlimVM.h"

namespace Slim {

    void loadVirtualSystem() {
        g_psxMachine = std::make_unique<VirtualMachine>();

    }

}

// Called from JNI native libraries loader, after load our library!
// We can initialize everything here before the application ends up it's Splash Screen
__attribute__((visibility("default"))) extern "C" jint
JNI_OnLoad([[maybe_unused]] JavaVM* vm, [[maybe_unused]] void* reserved) {
    const auto jniVersion{JNI_VERSION_1_6};

    // Creating our emulator's virtual machine instance while the app is loading!
    Slim::loadVirtualSystem();

    return jniVersion;
}

