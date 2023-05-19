// This source code is part of SlimNow project
#include <fmt/format.h>
#include <tasks/executor.h>

#include <utility>

namespace slim::tasks {
    static thread_local pthread_key_t workerKey;
    static thread_local auto hasAttached{false};

    std::unique_ptr<UnorderedExecutor> g_taskSolver;

    void UnorderedExecutor::workerInnerLoop(WorkerContext& worker,
                                            std::shared_ptr<ExecutorShared>& shared) {
        std::unique_lock<std::mutex> sharedGuard(shared->m_uniqueMutex);

        auto waitWorker = [&sharedGuard, &shared, &worker]() {
            shared->m_running--;
            worker.state = WorkerState::Waiting;

            auto runWorker = [&shared, &worker]() {
                if (worker.state == WorkerState::Stopped) {
                    pthread_exit(nullptr);
                }

                shared->m_running++;
                worker.state = WorkerState::Running;
            };

            // Waiting for a task comes to execute
            shared->m_condVar.wait(sharedGuard, [&shared, &worker]() {
                const auto &tasks{shared->m_tasks};
                return !(tasks.empty() && worker.state != WorkerState::Stopped);
            });

            runWorker();
        };
        // We're waiting for a condition to keep going on
        waitWorker();

        auto& taskList{shared->m_tasks};
        auto taskSolver{std::move(taskList.back())};
        taskList.pop_back();

        // It's unknown the time that the method will run, so we needed to release the lock
        // before continues
        sharedGuard.unlock();
        auto solved{taskSolver->m_method(shared->m_sharedVM)};
        if (taskSolver->m_futureRet != nullptr) {
            taskSolver->m_futureRet->set_value(solved);
        }

    }

    [[noreturn]] void UnorderedExecutor::workerRoutine(WorkerContext& worker,
                                                       std::shared_ptr<ExecutorShared>& shared) {
        auto nativeName{fmt::memory_buffer()};
        fmt::format_to(std::back_inserter(nativeName),
                       "Worker: {}", worker.thNumberId);

        pthread_setname_np(pthread_self(), nativeName.data());

        // Our worker will wait until a run signal is delivery
        std::unique_lock<std::mutex> workerGuard(worker.wMutex);
        const auto vm{shared->m_sharedVM};

        // Attaching our worker thread into our Java Virtual Environment
        if (vm->GetEnv(reinterpret_cast<void **>(&worker.thInterface),
                       JNI_VERSION_1_6) == JNI_EDETACHED) {
            vm->AttachCurrentThread(&worker.thInterface, nullptr);
            hasAttached = true;
        }

        pthread_key_create(&workerKey, onNativeExit);
        pthread_setspecific(hasAttached, vm);

        while (true) {
            workerInnerLoop(worker, shared);
        }

    }

    UnorderedExecutor::UnorderedExecutor(JavaVM *vMachine) {

        m_shared = std::make_shared<ExecutorShared>(vMachine);
        std::unique_lock<std::mutex> guard(m_shared->m_uniqueMutex);

        for (auto& context: m_contextArray) {
            static uint threadId{0};
            context.thNumberId = threadId++;
            context.state = WorkerState::Disable;
        }

        auto& firstWorker{m_contextArray[0]};

        guard.unlock();
        workerEnable(firstWorker);
    }

    UnorderedExecutor::~UnorderedExecutor() {
        std::unique_lock<std::mutex> guard(m_shared->m_uniqueMutex);

        for (auto &workerContext: m_contextArray) {
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
            context.state = WorkerState::Enabled;
            workerRoutine(context, m_shared);
        });
    }

    void UnorderedExecutor::workerDisable(WorkerContext& context) {
        std::unique_lock<std::mutex> guard(context.wMutex);

        if (context.state == WorkerState::Disable)
            return;
        else if (context.state == WorkerState::Running)
            m_shared->m_running--;

        context.state = WorkerState::Stopped;

        if (context.thHandler.joinable())
            context.thHandler.join();

        context.state = WorkerState::Disable;
    }

    [[maybe_unused]] void UnorderedExecutor::pushTaskForResult(task method,
                                                               futureResult futureValue) {
        auto& tasks{m_shared->m_tasks};

        std::unique_lock<std::mutex> global(m_shared->m_uniqueMutex);
        auto sharedTask{std::make_unique<WorkerTask>()};

        sharedTask->m_method = std::move(method);
        sharedTask->m_futureRet = std::move(futureValue);

        tasks.push_back(std::move(sharedTask));

    }

    void UnorderedExecutor::pushTask(task method) {
        auto& tasks{m_shared->m_tasks};

        std::unique_lock<std::mutex> global(m_shared->m_uniqueMutex);
        auto sharedTask{std::make_unique<WorkerTask>()};
        sharedTask->m_method = std::move(method);
        tasks.push_back(std::move(sharedTask));

    }


}

