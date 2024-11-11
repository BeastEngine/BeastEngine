#pragma once
#include "Beast/Common/Types.h"
#include "Beast/Common/AllocatorAwareId.h"

#include <vector>

namespace be
{
    /**
     * @brief Allocator generating AllocatorAwareIds.
     * The given id will be encoded into the generated Ids.
     * It effectively implements the Generational Index pattern.
     * This means that the allocated ids encode index, version (generation) and allocator id.
     * 
     * Note: This class is NOT thread-safe.
     */
    class IdAllocator
    {
    public:
        enum class ID : uint8
        {
            ID_0 = 0,
            ID_1 = 1,
            ID_2 = 2,
            ID_3 = 3,
            ID_4 = 4,
            ID_5 = 5,
            ID_6 = 6,
            ID_7 = 7,
            ID_8 = 8,
            ID_9 = 9,
            ID_10 = 10,
            ID_11 = 11,
            ID_12 = 12,
            ID_13 = 13,
            ID_LAST = 14,
        };

    public:
        explicit IdAllocator(ID id);

        /**
         * @brief Allocates id.
         * Will reuse previously deallocated ids first. Internally, it stores a free list with deallocated ids and pops ids from there before allocating a new one.
         * If there is available id to reuse, its index and allocator id will remain the same, but the version will be increased.
         */
        [[nodiscard]] AllocatorAwareId Allocate();
        void Deallocate(AllocatorAwareId id);

        // TODO: Write tests for this and add docs.
        uint32 ExtractIndex(AllocatorAwareId id) const;

        /**
         * @brief Checks if the given id:
         *  - Has index within the range of existing ids withing that allocator;
         *  - Is not AAID_NULL;
         *  - Has version and allocator id the same as the id stored on the given index withing that allocator.
         */
        [[nodiscard]] bool IsValid(AllocatorAwareId id) const;

    private:
        std::vector<AllocatorAwareId> m_ids;
        std::vector<AllocatorAwareId> m_freeList;
        ID m_id;
    };
} // namespace be
