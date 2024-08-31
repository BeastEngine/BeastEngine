#pragma once
#include <array>
#include <stdexcept>
#include <utility>

namespace be
{
    /**
     * @brief Simple constexpr version of the map.
     * Inspired by Json Turner's constexpr map shown here: https://www.youtube.com/watch?v=INn3xa4pMfg.
     * Should be used for very small data sets.
     */
    template<typename KeyType, typename ValueType, std::size_t MapSize>
    struct ConstexprMap
    {
        using StorageType = typename std::array<std::pair<KeyType, ValueType>, MapSize>;
        StorageType data;

        [[nodiscard]] constexpr ValueType At(const KeyType& key) const
        {
            const auto conditionFunctions = [&key](const auto& currentPair) constexpr {
                return currentPair.first == key;
            };

            const auto foundValue = std::find_if(data.begin(), data.end(), conditionFunctions);
            if (foundValue != data.end())
            {
                return foundValue->second;
            }

            throw std::range_error("Given Key not found in the map");
        }
    };
} // namespace be
