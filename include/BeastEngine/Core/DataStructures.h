#pragma once

namespace be
{
    template<typename KeyType, typename ValueType, std::size_t MapSize>
    struct ConstexprMap
    {
        using StorageType = typename std::array<std::pair<KeyType, ValueType>, MapSize>;
        StorageType data;

        [[nodiscard]] constexpr ValueType At(const KeyType& key) const
        {
            const auto conditionFunctions = [&key](const auto& currentPair) {
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
