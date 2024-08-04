#pragma once
#include <functional>
#include <vector>
#include <string_view>

namespace be
{
    class SystemFunction;

    class Schedule
    {
    public:
        explicit Schedule(std::vector<SystemFunction*> functions);
        const std::vector<SystemFunction*>& GetFunctions() const;

    private:
        std::vector<SystemFunction*> m_functions;
    };
}
