#pragma once
#include "Beast/Common/Utils/Hasher.h"

#include <memory>
#include <string_view>
#include <filesystem>
#include <cstdint>
#include <string>
#include <map>
#include <unordered_map>
#include <chrono>
#include <map>
#include <string>
#include <vector>

namespace be
{
    /*********************BASIC TYPES DEFINITIONS*************************/
    using byte = std::byte;

    // Note: in case of some weird type errors or speed issues, try replacing those with *_fast_* versions
    // See: https://en.cppreference.com/w/cpp/header/cstdint for reference
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;

    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    /*********************************************************************/

    /***********************POINTERS DEFINITIONS**************************/
    template<typename T>
    using Unique = std::unique_ptr<T>;

    template<typename T, typename... Args>
    [[nodiscard]] constexpr Unique<T> MakeUnique(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T, typename... Args>
    [[nodiscard]] constexpr Shared<T> MakeShared(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Weak = std::weak_ptr<T>;
    /***********************************************************************/

    /*******************************OTHERS**********************************/
    inline constexpr Id ID_EMPTY = Id{0};

    using TimestampDuration = std::chrono::nanoseconds;
    using Timestamp = TimestampDuration::rep;

    template<typename Enum>
    using UnderlyingType = std::underlying_type_t<Enum>;

    /**
     * Converts given enum into its underlying type.
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
     * Converts given value into the requested enum. 
     *  Requested Enum type is provided via template parameter.
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
    /***********************************************************************/
} // namespace be
