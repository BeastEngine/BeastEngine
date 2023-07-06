#pragma once
#include <Beast/Ecs/System.h>
#include <Beast/Ecs/World.h>

#include <Beast/Common/Types.h>

#include <entt/process/scheduler.hpp>

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <format>

namespace be
{
    class SystemsScheduler final
    {
        using TaskRunner = entt::scheduler<uint32>;
        using SystemsRegistry = std::unordered_set<std::type_index>;

    public:
        class Group
        {
            friend class SystemsScheduler;
            using PrepareSystemFunc = std::function<void(TaskRunner&)>;

        public:
            template<ecs_system System, typename... Args>
            void AttachSystem(Args&&... args)
            {
                const auto& id = typeid(System);
                if (m_systemsRegistry.contains(id))
                {
                    throw std::runtime_error(std::format("'{}' system is already attached!", id.name()));
                }

                m_systemsRegistry.insert(id);
                PrepareSystemFunc prepareFunction = [this, ... args = std::forward<Args>(args)](TaskRunner& runner) mutable {
                    PrepareSystem(MakeUnique<System>(args...), runner);
                };

                m_prepareFunctions.push_back(std::move(prepareFunction));
            }

        private:
            Group(SystemsRegistry& systemsRegistry)
                : m_systemsRegistry(systemsRegistry)
            {}

            void Prepare(TaskRunner& runner)
            {
                for (const auto& task : m_prepareFunctions)
                {
                    task(runner);
                }
            }

            template<ecs_system System>
            static void PrepareSystem(Unique<System> system, TaskRunner& runner)
            {
                auto task = [system = std::move(system)](uint32, void* data, auto, auto) {
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
        bool m_isLocked = false;
    };
} // namespace be