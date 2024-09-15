#include "Beast/Common/IdAllocatorsFactory.h"

namespace be
{
    IdAllocatorsFactory::IdAllocatorsFactory(std::size_t numberOfAllocators)
        : m_numberOfAllocators(numberOfAllocators)
    {
    }
} // namespace be
