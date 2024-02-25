#pragma once
#include "Beast/Common/Utils/Hasher.h"

#include <limits>
#include <string_view>
#include <string>

namespace be
{
    using RawId = HashType;
    constexpr static RawId INVALID_ID = std::numeric_limits<RawId>::max();

    class Id
    {
    public:
        constexpr Id() noexcept = default;
        
        constexpr explicit Id(RawId value) noexcept
            : m_value(value)
        {}
        
        constexpr explicit Id(std::string_view stringValue) noexcept
            : m_value(Hash(stringValue))
        {}

        constexpr RawId Raw() const noexcept
        {
            return m_value;
        }

        std::string ToString() const
        {
            return std::to_string(m_value);
        }

        constexpr bool operator==(const Id& other) const
        {
            return m_value == other.m_value;
        }

    private:
        RawId m_value = INVALID_ID;
    };

    static constexpr Id ID_EMPTY = Id{};
} // namespace be
