#pragma once
#include "Beast/Common/Utils/Hasher.h"

#include <limits>
#include <string_view>
#include <string>

namespace be
{
    using RawId = HashType;
    constexpr static RawId INVALID_ID = std::numeric_limits<RawId>::max();

    /**
     * @brief Represents an ID of any form.
     * It does not guarantee uniqueness.
     */
    class [[nodiscard]] Id
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
         * @brief Converts the underlying value to a string representation.
         */
        std::string ToString() const
        {
            return std::to_string(m_value);
        }

        constexpr bool operator==(const Id& other) const
        {
            return m_value == other.m_value;
        }

    public:
        struct Hasher
        {
            constexpr std::size_t operator()(const Id& id) const
            {
                static_assert(sizeof(RawId) <= sizeof(std::size_t), "Id is too big to fit in size to - thus cannot be used as a hash");
                return static_cast<std::size_t>(id.Raw());
            }
        };

    private:
        RawId m_value = INVALID_ID;
    };

    static constexpr Id ID_EMPTY = Id{};

} // namespace be
