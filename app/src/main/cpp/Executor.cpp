#include <fmt/format.h>

#include <Executor.h>

namespace Slim::Thread {
    static thread_local pthread_key_t workerKey;
    static thread_local auto hasAttached{false};

    std::unique_ptr<UnorderedExecutor> g_taskSolver;

    void UnorderedExecutor::workerInnerLoop(WorkerContext& worker,
                                            std::shared_ptr<ExecutorShared>& shared) {
        std::unique_lock<std::mutex> sharedGuard(shared->m_exeMutex);

        if (worker.workerState == WorkerContext::Stopped) {
            pthread_exit({});
        }

        // We're waiting for a condition
        shared->m_waiting++;
        // Waiting for a task comes to execute
        shared->m_exeCond.wait(sharedGuard, [&shared]() {
            auto& tasks{shared->m_tasks};
            return !tasks.empty();
        });
        shared->m_waiting--;

        auto& task{shared->m_tasks.back()};
        shared->m_tasks.pop_back();
        sharedGuard.unlock();

        // It's doesn't know the time that the method will run, so we needed to release the lock
        // before continues
        task.m_method(shared->m_sharedVM);
        task.m_finished = true;
    }

    [[noreturn]] void UnorderedExecutor::workerRoutine(WorkerContext& worker,
                                                       std::shared_ptr<ExecutorShared>& shared) {
        auto nativeName = fmt::memory_buffer();
        fmt::format_to(std::back_inserter(nativeName), "Worker: {}", worker.thNumberId);

        pthread_setname_np(pthread_self(), nativeName.data());

        // Our worker will wait until a run signal is delivery
        std::unique_lock<std::mutex> workerGuard(worker.wMutex);
        auto vm{shared->m_sharedVM};

        // Attaching our worker thread into our Java Virtual Environment
        if (vm->GetEnv(reinterpret_cast<void **>(&worker.thInterface),
                       JNI_VERSION_1_6) == JNI_EDETACHED) {
            vm->AttachCurrentThread(&worker.thInterface, nullptr);
            hasAttached = true;
        }

        pthread_key_create(&workerKey, onNativeExit);
        pthread_setspecific(hasAttached, vm);

        worker.workerState = WorkerContext::Running;
        shared->m_running++;

        while (true) { workerInnerLoop(worker, shared); }
    }

    UnorderedExecutor::UnorderedExecutor(JavaVM *vMachine) {

        m_shared = std::make_shared<ExecutorShared>(vMachine);
        std::unique_lock<std::mutex> guard(m_shared->m_exeMutex);

        for (auto& context: contextArray) {
            static uint threadId{0};
            context.thNumberId = threadId++;
            context.workerState = WorkerContext::Disable;
        }

        auto& firstWorker{contextArray[0]};

        guard.unlock();
        workerEnable(firstWorker);
    }

    UnorderedExecutor::~UnorderedExecutor() {
        std::unique_lock<std::mutex> guard(m_shared->m_exeMutex);

        for (auto &workerContext: contextArray) {
            workerDisable(workerContext);
        }

    }

    void UnorderedExecutor::onNativeExit(void* javaVM) {
        auto vm{reinterpret_cast<JavaVM *>(javaVM)};
        vm->DetachCurrentThread();

        // If we reach here, we already have a valid pthread key!
        pthread_key_delete(workerKey);
    }

    void UnorderedExecutor::workerEnable(WorkerContext &context) {
        std::unique_lock<std::mutex> guard(context.wMutex);

        context.thHandler = std::thread([&context, this]() {
            context.workerState = WorkerContext::Enabled;
            workerRoutine(context, m_shared);
        });
    }

    void UnorderedExecutor::workerDisable(WorkerContext& context) {
        std::unique_lock<std::mutex> guard(context.wMutex);

        if (context.workerState == WorkerContext::Disable)
            return;
        else if (context.workerState == WorkerContext::Running)
            m_shared->m_running--;

        context.workerState = WorkerContext::Stopped;

        if (context.thHandler.joinable())
            context.thHandler.join();

        context.workerState = WorkerContext::Disable;
    }

    void UnorderedExecutor::scheduleTask(task method) {
        auto& tasks{m_shared->m_tasks};

        std::unique_lock<std::mutex> global(m_shared->m_exeMutex);
        auto& shTask{tasks.emplace_back()};
        shTask.m_method = std::move(method);
    }

}

