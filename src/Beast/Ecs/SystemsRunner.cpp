#include "Beast/Ecs/SystemsRunner.h"
#include "Beast/Ecs/SystemFunction.h"

#include "Beast/Debug.h"

namespace be
{
    FunctionsQueue::FunctionsQueue(std::size_t numberOfThreads, World& world, std::function<void(SystemFunction*)> onFunctionRun)
        : m_world(world), m_onFunctionRun(onFunctionRun)
    {
        for (std::size_t i = 0; i < numberOfThreads; ++i)
        {
            m_threads.emplace_back([this]() {
                while (true)
                {
                    SystemFunction* function = nullptr;
                    {
                        std::unique_lock lock{m_queue_lock};
                        m_cv.wait(lock, [this] {
                            return !m_functions.empty() || m_shouldStop;
                        });

                        if (m_shouldStop)
                        {
                            return;
                        }

                        function = m_functions.front();
                        m_functions.pop();
                    }

                    function->Run(m_world);
                    m_onFunctionRun(function);
                }
            });
        }
    }

    FunctionsQueue::~FunctionsQueue()
    {
        {
            std::scoped_lock lock{m_queue_lock};
            m_shouldStop = true;
        }

        m_cv.notify_all();
    }

    void FunctionsQueue::Add(SystemFunction* function)
    {
        {
            std::scoped_lock lock{m_queue_lock};
            m_functions.push(function);
        }

        m_cv.notify_one();
    }

    SystemsRunner::SystemsRunner(World& world, SystemsSchedule schedule)
        : m_world(world), m_queue(std::thread::hardware_concurrency(), m_world, CreateFunctionCallback()), m_schedule(std::move(schedule))
    {
        for (auto& fn : m_schedule.functions)
        {
            m_parentsCounters[&fn] = 0;
        }
    }

    void SystemsRunner::Run()
    {
        m_isGraphCompleted = false;
        m_functionsLeftToRun = m_schedule.functions.size();

        for (SystemFunction* function : m_schedule.starterFunctions)
        {
            m_queue.Add(function);
        }
     
        // This will return if the value is already "true", so no need to worry that the functions finish running before we get here and hang forever.
        m_isGraphCompleted.wait(false);
    }

    std::function<void(SystemFunction*)> SystemsRunner::CreateFunctionCallback()
    {
        return [this](SystemFunction* fn) {
            if (--m_functionsLeftToRun == 0)
            {
                BE_ASSERT_MSG_ALWAYS(
                    fn->Children().size() == 0,
                    "The Systems Queue reported"
                    "that all functions have been run,"
                    "but there still seem to be functions left."
                );

                m_isGraphCompleted = true;
                m_isGraphCompleted.notify_all();

                return;
            }

            for (auto* child : fn->Children())
            {
                // This should be thread safe as all entries are already created in the constructor.
                auto& value = m_parentsCounters[child];
                ++value;
                if (value.load() == child->Parents().size())
                {
                    value = 0; // Reset for the future run
                    m_queue.Add(child);
                }
            }
        };
    }

    // TODO: Add another scheduler implementation that will just run all functions sequentially
    // instead of doing the multithreading mumbo jumbo.
    // It might turn out to be much faster in real life, but we need a way to measure both, so
    // I need to have a possibility of having both scenarios to choose from.
    // Also, having sequential schedule, gets rid of all potential data races which we also have to account for somehow.

    // Another idea would be to run all functions concurently but instead lock on the components accesses.
    // It would be interesting to see how all these 3 compare, because the one we have right now is definitely the most complicated.
} // namespace be
