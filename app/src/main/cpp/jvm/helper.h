// This source code is part of SlimNow project
#pragma once

#include <thread>
#include <array>
#include <vector>
#include <random>
#include <future>

#include <jni.h>
#include <sched.h>

namespace slim::jvm {

    using u8 = std::uint8_t;
    using task = std::function<jobject(JavaVM* virtualMachine)>;

    using futureResult = std::shared_ptr<std::promise<jobject>>;

    // This will help us to keep track of our workers finite states
    enum WorkerState {
        Disable, Enabled, Stopped, Waiting, Running
    };

}
