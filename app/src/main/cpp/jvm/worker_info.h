
// This source code is part of SlimNow project
#pragma once

#include <jvm/helper.h>

namespace slim::jvm {

    struct WorkerContext {
        u8 thNumberId;
        WorkerState state;

        JNIEnv* thInterface;
        std::thread thHandler;

        std::condition_variable wCond;
        std::mutex wMutex;
    };
    static constexpr u8 THREADS_MAX_COUNT {8 - 1};

    class WorkerTask {
    public:
        WorkerTask() {
            static std::random_device dev;
            static std::mt19937 rng(dev());
            static std::uniform_int_distribution
                    <std::mt19937::result_type> distAlgo(1, UINT_MAX);

            m_rndValue = distAlgo(dev);
            m_crCore = sched_getcpu();
        }
        // Can't copy or move this task object
        WorkerTask(WorkerTask &&worker) = delete;
        WorkerTask(WorkerTask &worker) = delete;

        auto operator==(const WorkerTask &&cmpTask) {
        return m_rndValue == cmpTask.m_rndValue &&
               m_crCore == cmpTask.m_crCore;
        }

        // A random value to help with task search
        uint m_rndValue;
        // Determine the core where this task has created at
        uint m_crCore;
        task m_method{};
        futureResult m_futureRet;
    };

    using sharedTask = std::unique_ptr<WorkerTask>;

}
