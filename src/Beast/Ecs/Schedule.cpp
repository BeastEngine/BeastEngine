#include "Beast/Ecs/Schedule.h"
#include "Beast/Ecs/SystemFunction.h"

namespace be
{
    Schedule::Schedule(std::vector<SystemFunction*> functions)
        : m_functions(std::move(functions))
    {
    }
    
    const std::vector<SystemFunction*>& Schedule::GetFunctions() const
    {
        return m_functions;
    }
} // namespace be
