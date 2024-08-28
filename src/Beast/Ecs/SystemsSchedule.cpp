#include "Beast/Ecs/SystemsSchedule.h"
#include "Beast/Ecs/SystemFunction.h"

namespace be
{
    SystemsSchedule::SystemsSchedule(std::vector<SystemFunction*> starterFunctions, std::size_t numberOfFunctions)
        : m_functions(std::move(starterFunctions)), m_numberOfFunctions(numberOfFunctions)
    {
    }
    
    const std::vector<SystemFunction*>& SystemsSchedule::GetStarterFunctions() const
    {
        return m_functions;
    }

    std::size_t SystemsSchedule::GetNumberOfFunctions() const
    {
        return m_numberOfFunctions;
    }
} // namespace be
