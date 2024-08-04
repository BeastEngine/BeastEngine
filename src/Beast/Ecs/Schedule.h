#pragma once
#include <functional>
#include <vector>
#include <string_view>

namespace be
{
    class World;
    class SystemFunction;

    class ScheduledFunction
    {
    public:
        std::string_view Name() const;
        const std::vector<SystemFunction*>& Parents() const;
        const std::vector<SystemFunction*>& Children() const;

    private:
        SystemFunction& m_function;
    };

    class Schedule
    {
    public:
        explicit Schedule(std::vector<ScheduledFunction> functions);
        const std::vector<ScheduledFunction>& GetFunctions() const;

    private:
        std::vector<ScheduledFunction> m_functions;
    };
}
