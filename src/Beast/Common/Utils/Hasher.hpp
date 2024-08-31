#pragma once
#include "Beast/Common/Helpers.hpp"
#include "Beast/Common/Utils/Xxh64.hpp"

#include <xxhash.h>
#include <string_view>
#include <span>

namespace be
{
    constexpr inline XXH64_hash_t SEED = 777;
    using HashType = XXH64_hash_t;

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
    inline HashType Hash(T&& valueToHash)
    {
        return XXH64(valueToHash.data(), valueToHash.size(), SEED);
    }

    constexpr inline HashType Hash(std::string_view input)
    {
        return xxh64::hash(input.data(), input.size(), SEED);
    }
} // namespace be
