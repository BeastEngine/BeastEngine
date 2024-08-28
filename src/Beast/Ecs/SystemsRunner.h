#pragma once
#include "Beast/Common/Id.h"

#include <mutex>
#include <vector>
#include <queue>
#include <stop_token>
#include <thread>
#include <cstdint>
#include <atomic>
#include <functional>
#include <future>

namespace be
{
    class SystemsSchedule;
    class SystemFunction;
    class World;

    class FunctionsQueue
    {
    public:
        FunctionsQueue(std::size_t numberOfThreads, World& world, std::function<void(SystemFunction*)> onFunctionRun);
        ~FunctionsQueue();

        void Add(SystemFunction* function);

    private:
        World& m_world;

        std::mutex m_queue_lock;
        std::condition_variable m_cv;
        bool m_shouldStop = false;

        std::queue<SystemFunction*> m_functions;
        std::vector<std::jthread> m_threads;
        std::function<void(SystemFunction*)> m_onFunctionRun;
    };

    class SystemsRunner
    {
    public:
        SystemsRunner(World& world);
        void Run(const SystemsSchedule& schedule);

    private:
        std::function<void(SystemFunction*)> CreateFunctionCallback();

    private:
        World& m_world;
        FunctionsQueue m_queue;
        
        std::atomic_size_t m_functionsLeftToRun;
        std::atomic_bool m_isGraphCompleted = false;

        std::unordered_map<SystemFunction*, std::atomic_size_t> m_parentsCounters;
    };
}
