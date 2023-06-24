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

    void SystemsScheduler::Prepare(std::vector<Group>& groups)
    {
        m_runners.reserve(groups.size());
        for (auto& group : groups)
        {
            m_runners.emplace_back();
            group.Prepare(m_runners.back());
        }
    }

    void SystemsScheduler::Update()
    {
        for (auto& runner : m_runners)
        {
            runner.update(0, reinterpret_cast<void*>(&m_world));
        }
    }
} // namespace be