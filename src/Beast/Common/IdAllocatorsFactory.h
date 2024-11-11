#pragma once
#include "Beast/Common/IdAllocator.h"
#include "Beast/Common/AllocatorAwareId.h"
#include "Beast/Common/Id.h"

#include <array>

namespace be
{
    class IdAllocatorsFactory
    {
    public:
        IdAllocatorsFactory();
        [[nodiscard]] IdAllocator* TryGet(IdAllocator::ID allocatorId);

    private:
        std::array<IdAllocator, ToUnderlying(IdAllocator::ID::ID_LAST) + 1> m_allocators;
    };
} // namespace be
