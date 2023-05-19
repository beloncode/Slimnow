// This source code is part of SlimNow project
#pragma once

#include <tasks/worker_info.h>

namespace slim::tasks {
    class UnorderedExecutor {

        class ExecutorShared {
        public:
            ExecutorShared(JavaVM* vm): m_sharedVM(vm) {}
            std::condition_variable m_condVar;
            std::mutex m_uniqueMutex;

            std::vector<sharedTask> m_tasks;
            JavaVM* m_sharedVM;
            uint m_running;
        };
    public:
        explicit UnorderedExecutor(JavaVM* vMachine);
        [[maybe_unused]] void pushTaskForResult(task method, futureResult futureValue);
        void pushTask(task method);

        ~UnorderedExecutor();

    private:
        void workerEnable(WorkerContext& context);
        void workerDisable(WorkerContext& context);
        [[noreturn]] static void workerRoutine(WorkerContext& worker,
                                               std::shared_ptr<ExecutorShared>& shared);
        static void workerInnerLoop(WorkerContext& worker,
                                    std::shared_ptr<ExecutorShared>& shared);

        static void onNativeExit(void* JavaVM);

        std::array<WorkerContext, THREADS_MAX_COUNT> m_contextArray;
        std::shared_ptr<ExecutorShared> m_shared;

    };

    extern std::unique_ptr<UnorderedExecutor> g_taskSolver;
}

