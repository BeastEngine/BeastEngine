#pragma once
#include <Beast/Ecs/System.h>
#include <Beast/Ecs/World.h>

#include <Beast/Common/Types.h>

#include <entt/process/scheduler.hpp>

#include <typeindex>
#include <initializer_list>
#include <vector>

namespace be
{
    class Task : public entt::process<Task, uint32>
    {
    public:
        using Callback = std::function<void(World&)>;

    public:
        Task(Callback callback)
            : m_callback(std::move(callback))
        {}

        void update(delta_type, void* data)
        {
            auto* world = reinterpret_cast<World*>(data);
            m_callback(*world);
        }

    private:
        Callback m_callback;
    };

    class SystemsScheduler final
    {
        using SystemsRegistry = std::unordered_map<std::type_index, Unique<ISystem>>;

    public:
        class Group
        {
            friend class SystemsScheduler;

        public:
            Group(SystemsRegistry& systemsRegistry)
                : m_systemsRegistry(systemsRegistry)
            {}

            template<ecs_system System, typename... Args>
            void AttachSystem(Args&&... args)
            {
                auto newSystem = MakeUnique<System>(std::forward<Args>(args)...);
                auto* systemInstance = newSystem.get();

                m_systemsRegistry[std::type_index(typeid(System))] = std::move(newSystem);
                Task::Callback taskCallback = [system = systemInstance](World& world) {
                    const auto view = world.CreateView<System::AccessList>();
                    system->Run(view);
                };

                m_callback.push_back(std::move(taskCallback));
            }

        private:
            void Prepare(entt::scheduler<uint32>& runner) const
            {
                for (const auto& callback : m_callback)
                {
                    runner.attach<Task>(callback);
                }
            }

        private:
            SystemsRegistry& m_systemsRegistry;
            std::vector<Task::Callback> m_callback;
        };

    public:
        SystemsScheduler(World& world);

        Group CreateGroup();
        void Prepare(std::initializer_list<Group> groups);
        void Update();

    private:
        World& m_world;
        SystemsRegistry m_systems;
        std::vector<entt::scheduler<uint32>> m_runners;
    };
} // namespace be