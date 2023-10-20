#pragma once
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Utils/Xxh64.h"

#include <xxhash.h>
#include <string_view>
#include <span>

namespace be
{
    constexpr inline XXH64_hash_t SEED = 777;
    enum class Id : XXH64_hash_t
    {
    };

    /**
     * @brief Hashes given value using XXH64 hash function.
     * It returns the same hash for the same \p input
     * 
     * @tparam T - Type that provides data() and size().
     * 
     * @param input - Value To hash
     * @return Hash representation of the given value
     */
    template<typename T>
    inline Id Hash(T&& valueToHash)
    {
        return Id{XXH64(valueToHash.data(), valueToHash.size(), SEED)};
    }

    constexpr inline Id Hash(std::string_view input)
    {
        return Id{xxh64::hash(input.data(), input.size(), SEED)};
    }

    constexpr inline Id ID(std::string_view id)
    {
        return Hash(id);
    }
} // namespace be
