#pragma once
#include "Beast/Common/Utils/Hasher.hpp"

#include <uuid.h>

#include <limits>
#include <string_view>
#include <string>
#include <random>

namespace be
{
    using RawId = HashType;
    constexpr static RawId INVALID_ID = std::numeric_limits<RawId>::max();

    /**
     * @brief Represents an ID of any form.
     * It does not guarantee uniquness.
     */
    class [[nodiscard]] Id final
    {
    public:
        /**
         * @brief Creates default, empty id.
         */
        constexpr Id() noexcept = default;

        constexpr explicit Id(RawId value) noexcept
            : m_value(value)
        {
        }

        /**
         * @brief Generates new Id by hashing the given \c stringValue.
         */
        constexpr explicit Id(std::string_view stringValue) noexcept
            : m_value(Hash(stringValue))
        {
        }

        /**
         * @brief Returns the underlying, raw value.
         */
        constexpr RawId Raw() const noexcept
        {
            return m_value;
        }

        /**
         * @brief Converts the undrelying value to a string representation.
         */
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
