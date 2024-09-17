#include "Beast/Common/IdAllocatorsFactory.h"
#include "Beast/Debug.h"

namespace be
{
    IdAllocatorsFactory::IdAllocatorsFactory()
        : m_allocators{
              IdAllocator{IdAllocator::ID::ID_0},
              IdAllocator{IdAllocator::ID::ID_1},
              IdAllocator{IdAllocator::ID::ID_2},
              IdAllocator{IdAllocator::ID::ID_3},
              IdAllocator{IdAllocator::ID::ID_4},
              IdAllocator{IdAllocator::ID::ID_5},
              IdAllocator{IdAllocator::ID::ID_6},
              IdAllocator{IdAllocator::ID::ID_7},
              IdAllocator{IdAllocator::ID::ID_8},
              IdAllocator{IdAllocator::ID::ID_9},
              IdAllocator{IdAllocator::ID::ID_10},
              IdAllocator{IdAllocator::ID::ID_11},
              IdAllocator{IdAllocator::ID::ID_12},
              IdAllocator{IdAllocator::ID::ID_13},
              IdAllocator{IdAllocator::ID::ID_LAST},
          }
    {
    }

    IdAllocator& IdAllocatorsFactory::Get(IdAllocator::ID allocatorId)
    {
        BE_ASSERT_MSG(ToUnderlying(allocatorId) <= ToUnderlying(IdAllocator::ID::ID_LAST), "Tried to access allocator via invalid id: {}", ToUnderlying(allocatorId));
        return m_allocators[ToUnderlying(allocatorId)];
    }
} // namespace be
