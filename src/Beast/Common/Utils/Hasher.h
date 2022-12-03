#pragma once
#include <Beast/Common/Helpers.h>

namespace be
{
    using HashSeed = XXH64_hash_t;
    using HashValue = XXH64_hash_t;

    /**
     * @brief Provides static, hashing-related functionalities.
     */
    class Hasher final
    {
    public:
        CT_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(Hasher);

        /**
         * @brief Hashes given value using XXH64 hash function. 
         * Given value must provide data() and size() methods in order to be hashed.
         * 
         * @param valueToHash
         * @return Hash representation of the given value
         */
        template<typename T>
        static constexpr HashValue Hash(const T& valueToHash)
        {
            return XXH64(valueToHash.data(), valueToHash.size(), SEED);
        }

    private:
        static constexpr HashSeed SEED = 777;
    };
} // namespace be
