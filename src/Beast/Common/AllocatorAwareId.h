#pragma once
#include "Beast/Common/Types.h"

namespace be
{
    // Masks for extracting appropriate bits from an ID.
    inline constexpr uint32 AAID_INDEX_MASK = 0x7FFFF;
    inline constexpr uint32 AAID_VERSION_MASK = 0x1FF;
    inline constexpr uint8 AAID_ALLOCATOR_MASK = 0xF;

    // Starting bits of each of the segments of the ID.
    // Should be used to know how many bits to shift the values to construct/read ID.
    inline constexpr uint8 AAID_INDEX_START_BIT = 0u;
    inline constexpr uint8 AAID_VERSION_START_BIT = 19u;
    inline constexpr uint8 AAID_ALLOCATOR_START_BIT = 28u;

    enum class AllocatorAwareId : uint32
    {
    };

    // Represents the empty or null ID.
    inline constexpr auto AAID_NULL = AllocatorAwareId{0xFFFFFFFF};
} // namespace be
