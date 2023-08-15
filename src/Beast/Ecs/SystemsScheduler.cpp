#include <Beast/Ecs/SystemsScheduler.h>

namespace be
{
    SystemsScheduler::SystemsScheduler(World& world)
        : m_world(world)
    {
    }

    SystemsScheduler::Group SystemsScheduler::CreateGroup()
    {
        return Group(m_systems);
    }

    void SystemsScheduler::Prepare(const std::vector<Group>& groups)
    {
        if (m_isLocked)
        {
            throw std::runtime_error("The scheduler has already been prepared for run!");
        }

        m_runners.reserve(groups.size());
        for (const auto& group : groups)
        {
            m_runners.emplace_back();
            group.Prepare(m_runners.back());
        }

        m_isLocked = true;
    }

    void SystemsScheduler::Update()
    {
        for (auto& runner : m_runners)
        {
            runner.update(0, reinterpret_cast<void*>(&m_world));
        }
    }
} // namespace be