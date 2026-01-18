#pragma once
#include "Beast/Common/IdAllocator.h"
#include "Beast/Common/TypeTraits.h"
#include "Beast/Debug.h"

namespace be
{
    static constexpr AllocatorAwareId CreateFromParts(uint32 index, uint16 version, IdAllocator::ID allocator)
    {
        uint32 id = index;
        id |= version << AAID_VERSION_START_BIT;
        id |= ToUnderlying(allocator) << AAID_ALLOCATOR_START_BIT;

        return AllocatorAwareId{id};
    }

    static constexpr uint16 ExtractVersion(AllocatorAwareId id)
    {
        return (ToUnderlying(id) >> AAID_VERSION_START_BIT) & AAID_VERSION_MASK;
    }

    IdAllocator::IdAllocator(IdAllocator::ID id)
        : m_id(id)
    {
        const auto rawId = ToUnderlying(id);
        if (rawId > ToUnderlying(IdAllocator::ID::ID_LAST))
        {
            throw std::runtime_error(std::format("Given {} id is not a valid id!", rawId));
        }
    }

    AllocatorAwareId IdAllocator::Allocate()
    {
        if (m_freeList.empty())
        {
            const uint32 index = static_cast<uint32>(m_ids.size());
            BE_ASSERT_MSG_ALWAYS(index <= AAID_INDEX_MASK, "{}", "Tried to allocate new id, but there's no more available indices!");

            return m_ids.emplace_back(CreateFromParts(index, 0, m_id));
        }

        AllocatorAwareId id = m_freeList.back();
        m_freeList.pop_back();
        m_ids[ExtractIndex(id)] = id;

        return id;
    }

    void IdAllocator::Deallocate(AllocatorAwareId id)
    {
        if (!IsValid(id))
        {
            // TODO: Instead of throwing, we should maybe return something like DeallocationResult?
            // This is to check later, depending on the usage. It might be that it will be better to let the game decide if this should be a critical error or not.
            throw std::runtime_error(std::format("Given {} id is not valid!", ToUnderlying(id)));
        }

        // Extract id's current version and increase it by 1. This becomes a new revision
        const uint32 index = ExtractIndex(id);
        const uint16 newVersion = ExtractVersion(id) + 1;

        m_freeList.emplace_back(CreateFromParts(index, newVersion, m_id));
        m_ids[index] = AAID_NULL;
    }

    uint32 IdAllocator::ExtractIndex(AllocatorAwareId id) const
    {
        return ToUnderlying(id) & AAID_INDEX_MASK;
    }

    bool IdAllocator::IsValid(AllocatorAwareId id) const
    {
        // Make sure it's in the range. Otherwise it's definitely invalid.
        const auto index = ExtractIndex(id);
        if (index >= m_ids.size())
        {
            return false;
        }

        // If given ID is not null, we can just check if the id we store at the given id's index is the same as the given one.
        // If yes, it means they have index, version and allocator id the same. Otherwise, these values don't match.
        return id != AAID_NULL && id == m_ids[ExtractIndex(id)];
    }
} // namespace be
