// This source code is part of SlimNow project
#pragma once

#include <thread>
#include <array>
#include <vector>
#include <random>
#include <future>

#include <jni.h>
#include <sched.h>

namespace slim::tasks {
    class UnorderedExecutor {
        using u8 = std::uint8_t;
        using task = std::function<void(JavaVM* virtualMachine)>;

        struct WorkerContext {
            u8 thNumberId;
            enum {
                Disable, Enabled, Stopped, Running
            } workerState;
            JNIEnv* thInterface;
            std::thread thHandler;

            std::condition_variable wCond;
            std::mutex wMutex;
        };
        static constexpr u8 THREADS_MAX_COUNT{8};

        class WorkerTask {
        public:
            WorkerTask() {
                static std::random_device dev;
                static std::mt19937 rng(dev());
                static std::uniform_int_distribution<std::mt19937::result_type> distAlgo(1, UINT_MAX);

                m_rndValue = distAlgo(dev);
                m_crCore = sched_getcpu();
            }
            // Can't copy or move this task object
            WorkerTask(WorkerTask&& worker) = delete;
            WorkerTask(WorkerTask& worker) = delete;

            [[maybe_unused]] bool m_finished{};
            std::mutex m_taskMutex;
            std::condition_variable m_taskCond;

            auto operator==(const WorkerTask&& cmpTask) {
                return m_rndValue == cmpTask.m_rndValue &&
                    m_crCore == cmpTask.m_crCore;
            }
            // A random value to help with task search
            uint m_rndValue;
            // Determine the core where this task was created at
            uint m_crCore;
            task m_method{};
        };

        class ExecutorShared {
        public:
            ExecutorShared(JavaVM* vm): m_sharedVM(vm) {}
            std::condition_variable m_exeCond;
            std::mutex m_exeMutex;

            std::vector<std::unique_ptr<WorkerTask>> m_tasks;
            JavaVM* m_sharedVM;
            uint m_running;
            uint m_waiting;
        };
    public:
        explicit UnorderedExecutor(JavaVM* vMachine);
        void scheduleTask(task method);

        ~UnorderedExecutor();

    private:
        void workerEnable(WorkerContext& context);
        void workerDisable(WorkerContext& context);
        [[noreturn]] static void workerRoutine(WorkerContext& worker,
                                               std::shared_ptr<ExecutorShared>& shared);
        static void workerInnerLoop(WorkerContext& worker,
                                    std::shared_ptr<ExecutorShared>& shared);

        static void onNativeExit(void* JavaVM);

        std::array<WorkerContext, THREADS_MAX_COUNT> contextArray;
        std::shared_ptr<ExecutorShared> m_shared;

    };

    extern std::unique_ptr<UnorderedExecutor> g_taskSolver;
}
