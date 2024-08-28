#pragma once
#include "Beast/Ecs/SystemsSchedule.h"
#include "Beast/Ecs/SystemFunction.h"
#include "Beast/Ecs/World.h"

#include <string_view>
#include <vector>
#include <span>
#include <type_traits>

namespace be
{
    class Scheduler
    {
    public:
        template<typename... Views>
        using NonMemberFn = void (*)(Views...);

        template<typename System, typename... Views>
        using MemberFn = void (System::*)(Views...);

        template<typename... Views>
        void RegisterFunction(std::string_view name, NonMemberFn<Views...> systemFunction)
        {
            auto wrapperFunction = [function = std::move(systemFunction)](be::World& world) {
                function(world.CreateView<typename std::decay_t<Views>::AL>()...);
            };
            CreateFunction<Views...>(name, std::move(wrapperFunction));
        }

        template<typename System, typename... Views>
        void RegisterFunction(std::string_view name, MemberFn<System, Views...> systemFunction, System* systemInstance)
        {
            auto wrapperFunction = [function = std::move(systemFunction), system = systemInstance](be::World& world) {
                std::invoke(function, system, world.CreateView<typename std::decay_t<Views>::AL>()...);
            };
            CreateFunction<Views...>(name, std::move(wrapperFunction));
        }

        SystemsSchedule Prepare();

    private:
        template<typename... Views>
        void CreateFunction(std::string_view name, SystemFunction::Wrapper&& wrapper)
        {
            CheckFunctionUniquness(name);
            m_functions.emplace_back(SystemFunction::Create<Views...>(Id{m_functions.size()}, name, std::move(wrapper)));
        }

        void CheckFunctionUniquness(std::string_view name);
        void CheckDependencies(SystemFunction& lhs, SystemFunction& rhs);
        void ResolveDependencies(std::span<SystemFunction> functions);
        void SetUpStarterFunctions(std::vector<SystemFunction*>& functions);
        void VerifyGraphIsDAG(std::span<const SystemFunction> functions);

    private:
        std::vector<SystemFunction> m_functions;
        std::vector<SystemFunction*> m_starterFunctions;
    };
} // namespace be
