#pragma once
#include "Beast/Ecs/Types.h"
#include "Beast/Ecs/World.h"
#include "Beast/Ecs/View.h"

#include "Beast/Common/Types.h"

#include <entt/process/scheduler.hpp>

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <format>

namespace be
{
    template<typename T>
    concept ecs_system = requires {
        std::is_same<typename T::AccessList, be::BaseAccessList>::value;
    };

    class SystemsScheduler final
    {
        using TaskRunner = entt::scheduler;
        using SystemsRegistry = std::unordered_set<std::type_index>;

    public:
        class Group
        {
            friend class SystemsScheduler;
            using PrepareSystemFunc = std::function<void(TaskRunner&)>;

        public:
            template<typename System, typename... Args>
            void AttachSystem(Args&&... args)
            {
                const auto& id = typeid(System);
                if (m_systemsRegistry.contains(id))
                {
                    throw std::runtime_error(std::format("'{}' system is already attached!", id.name()));
                }

                m_systemsRegistry.insert(id);
                PrepareSystemFunc prepareFunction = [this, ... args = std::forward<Args>(args)](TaskRunner& runner) mutable {
                    PrepareSystem<System>(MakeUnique<System>(args...), runner, &System::Run);
                };

                m_prepareFunctions.push_back(std::move(prepareFunction));
            }

        private:
            Group(SystemsRegistry& systemsRegistry)
                : m_systemsRegistry(systemsRegistry)
            {}

            void Prepare(TaskRunner& runner) const
            {
                for (const auto& prepareTask : m_prepareFunctions)
                {
                    prepareTask(runner);
                }
            }

            // TODO: Add support for const system functions
            template<typename System, typename... Views>
            static void PrepareSystem(Unique<System> system, TaskRunner& runner, void (System::*fn)(Views...))
            {
                auto task = [system = std::move(system)](uint32, void* data, auto, auto) {
                    auto* world = reinterpret_cast<World*>(data);
                    system->Run(world->CreateView<typename std::decay_t<Views>::AL>()...);
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
        void Prepare(const std::vector<Group>& groups);
        void Update();

    private:
        World& m_world;
        SystemsRegistry m_systems;
        std::vector<TaskRunner> m_runners;
        bool m_isLocked = false;
    };
} // namespace be
