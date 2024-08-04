#pragma once
#include "Beast/Ecs/Schedule.h"
#include "Beast/Ecs/SystemFunction.h"
#include "Beast/Ecs/World.h"

#include <string_view>
#include <vector>
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
            CheckFunctionUniquness(name);

            auto wrapperFunction = [function = std::move(systemFunction)](be::World& world) {
                function(world.CreateView<typename std::decay_t<Views>::AL>()...);
            };
            m_functions.emplace_back(SystemFunction::Create<Views...>(Id{m_functions.size()}, name, std::move(wrapperFunction)));
        }

        template<typename System, typename... Views>
        void RegisterFunction(std::string_view name, MemberFn<System, Views...> systemFunction, System* systemInstance)
        {
            CheckFunctionUniquness(name);

            auto wrapperFunction = [function = std::move(systemFunction), system = systemInstance](be::World& world) {
                std::invoke(function, system, world.CreateView<typename std::decay_t<Views>::AL>()...);
            };
            m_functions.emplace_back(SystemFunction::Create<Views...>(Id{m_functions.size()}, name, std::move(wrapperFunction)));
        }

        Schedule Prepare();

    private:
        void CheckFunctionUniquness(std::string_view name);

    private:
        std::vector<SystemFunction> m_functions;
        std::vector<SystemFunction*> m_starterFunctions;
    };
} // namespace be
