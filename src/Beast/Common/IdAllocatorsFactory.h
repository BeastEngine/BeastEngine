#pragma once
#include "Beast/Common/IdAllocator.h"
#include "Beast/Common/AllocatorAwareId.h"
#include "Beast/Common/Id.h"

#include <vector>

namespace be
{
    enum class AllocatorIndex : uint8
    {
    };

    class IdAllocatorsFactory
    {
    public:
        explicit IdAllocatorsFactory(std::size_t numberOfAllocators);

        // This AllocatorIndex is weird. I will probably just use normal indices instead.
        IdAllocator& Get(AllocatorIndex);

    private:
        std::vector<IdAllocator> m_allocators;
        std::size_t m_numberOfAllocators;
    };
}
