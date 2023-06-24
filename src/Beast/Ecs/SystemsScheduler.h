#pragma once
#include <Beast/Ecs/System.h>
#include <Beast/Ecs/World.h>

#include <Beast/Common/Types.h>

#include <entt/process/scheduler.hpp>

#include <typeindex>
#include <initializer_list>
#include <span>
#include <vector>

namespace be
{    
    class SystemsScheduler final
    {
        using TaskRunner = entt::scheduler<uint32>;
        using SystemsRegistry = std::unordered_map<std::type_index, Unique<ISystem>>;

    public:
        class Group
        {
            friend class SystemsScheduler;
            using PrepareSystemFunc = std::function<void(Group&, TaskRunner&)>;

        public:
            Group(SystemsRegistry& systemsRegistry)
                : m_systemsRegistry(systemsRegistry)
            {}

            template<ecs_system System, typename... Args>
            void AttachSystem(Args&&... args)
            {
                auto newSystem = MakeUnique<System>(std::forward<Args>(args)...);
                m_systemsRegistry[std::type_index(typeid(System))] = std::move(newSystem);

                m_prepareFunctions.emplace_back(&PrepareSystem<System>);
            }

        private:
            void Prepare(TaskRunner& runner)
            {
                for (const auto& task : m_prepareFunctions)
                {
                    task(*this, runner);
                }
            }

            template<ecs_system System>
            static void PrepareSystem(Group& group, TaskRunner& runner)
            {
                Unique<ISystem>& baseSystem = group.m_systemsRegistry.at(std::type_index(typeid(System)));
                auto* system = static_cast<System*>(baseSystem.get());

                auto task = [system = system](uint32, void* data, auto, auto) {
                    auto* world = reinterpret_cast<World*>(data);
                    const auto view = world->CreateView<System::AccessList>();

                    system->Run(view);
                };
                runner.attach(std::move(task));
            }

        private:
            SystemsRegistry& m_systemsRegistry;
            std::vector<PrepareSystemFunc> m_prepareFunctions;
        };

    public:
        SystemsScheduler(World& world);

        Group CreateGroup();
        void Prepare(std::vector<Group>& groups);
        void Update();

    private:
        World& m_world;
        SystemsRegistry m_systems;
        std::vector<entt::scheduler<uint32>> m_runners;
    };
} // namespace be