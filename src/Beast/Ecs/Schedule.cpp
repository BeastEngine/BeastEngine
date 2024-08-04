#include "Beast/Ecs/Schedule.h"
#include "Beast/Ecs/SystemFunction.h"

namespace be
{
    Schedule::Schedule(std::vector<ScheduledFunction> functions)
        : m_functions(std::move(functions))
    {
    }
    
    const std::vector<ScheduledFunction>& Schedule::GetFunctions() const
    {
        return m_functions;
    }
    
    std::string_view ScheduledFunction::Name()
    {
        return m_function->m_name;
    }

    const std::vector<SystemFunction*> ScheduledFunction::Parents()
    {
        return m_function->m_parents;
    }

    const std::vector<SystemFunction*> ScheduledFunction::Children()
    {
        return m_function->m_children;
    }
} // namespace be
