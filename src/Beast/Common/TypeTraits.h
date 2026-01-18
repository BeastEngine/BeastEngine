#pragma once
#include <type_traits>

namespace be
{
    template<typename Enum>
    using UnderlyingType = std::underlying_type_t<Enum>;

    /**
     * @brief Converts given enum into its underlying type.
     * 
     * @param enumToConvert
     * @return 
     */
    template<typename Enum>
    constexpr auto ToUnderlying(Enum enumToConvert) noexcept
    {
        static_assert(std::is_enum_v<Enum>, "Given Enum Type is not an actual enum!");
        return static_cast<UnderlyingType<Enum>>(enumToConvert);
    }

    /**
     * @brief Converts given value into the requested enum. 
     * Requested Enum type is provided via template parameter.
     * 
     * @param valueToConvert
     * @return 
     */
    template<typename Enum, typename T>
    constexpr auto ToEnum(T valueToConvert) noexcept
    {
        static_assert(std::is_enum_v<Enum>, "Given Enum Type is not an actual enum!");
        const auto value = static_cast<UnderlyingType<Enum>>(valueToConvert);

        return static_cast<Enum>(value);
    }
} // namespace be