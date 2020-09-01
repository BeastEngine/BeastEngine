#pragma once
#include <spdlog/logger.h>
#include <glm/glm.hpp>

namespace be
{
    /*********************BASIC TYPES DEFINITIONS*************************/
    using byte = unsigned char;
    constexpr byte bit(byte x)
    {
        return (1 << x);
    };

    // Note: in case of some weird type errors or speed issues, try replacing those with *_fast_* versions
    // See: https://en.cppreference.com/w/cpp/header/cstdint for reference
    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    /*********************************************************************/

    /***********************POINTERS DEFINITIONS***************************/
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr SharedPtr<T> CreateSharedPtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr UniquePtr<T> CreateUniquePtr(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    /***********************************************************************/

    /*************************MATHS DEFINITIONS*****************************/
    using Vec2 = glm::vec2;
    using IntVec2 = glm::ivec2;
    /***********************************************************************/

    /*******************************OTHER**********************************/
    using ILogger = be::SharedPtr<spdlog::logger>;
    /***********************************************************************/
} // namespace be
