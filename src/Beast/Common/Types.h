#pragma once
#include <cstdint>
#include <chrono>

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
    
    /***********************************************************************/

    /*******************************OTHERS**********************************/
    using TimestampDuration = std::chrono::nanoseconds;
    using Timestamp = TimestampDuration::rep;
    /***********************************************************************/
} // namespace be
