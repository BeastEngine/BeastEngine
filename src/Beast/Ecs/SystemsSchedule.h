#pragma once
#include "Beast/Ecs/SystemFunction.h"

#include <vector>
#include <cstdint>

namespace be
{
    class SystemsSchedule
    {
    public:
        explicit SystemsSchedule(std::vector<SystemFunction*> starterFunctions, std::size_t numberOfFunctions);
        const std::vector<SystemFunction*>& GetStarterFunctions() const;
        std::size_t GetNumberOfFunctions() const;

    private:
        std::vector<SystemFunction*> m_functions;
        std::size_t m_numberOfFunctions;
    };
        
} // namespace be
