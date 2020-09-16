#include "DataStructuresTests.h"

#include <BeastEngine/Core/Types.h>
#include <BeastEngine/Core/DataStructures.h>

#include <array>

namespace be::tests::unit
{
    TEST_F(ConstexprMapTest, AtWillReturnValidElement)
    {
        const uint8 expectedValue = 5;
        const uint8 key = 1;
        const std::size_t mapSize = 1;

        constexpr std::array<std::pair<uint8, uint8>, mapSize> mapData{{{key, expectedValue}}};
        constexpr auto sut = ConstexprMap<uint8, uint8, mapSize>{mapData};
        ASSERT_EQ(expectedValue, sut.At(key));
    }

    TEST_F(ConstexprMapTest, AtWillThrowExceptionWhenGivenKeyNotFound)
    {
        const uint8 key = 1;
        const uint8 value = 5;
        const uint8 invalidKey = 2;
        const std::size_t mapSize = 1;

        constexpr std::array<std::pair<uint8, uint8>, mapSize> mapData{{{key, value}}};
        constexpr auto sut = ConstexprMap<uint8, uint8, mapSize>{mapData};
        ASSERT_THROW(
            const auto value = sut.At(invalidKey),
            std::range_error
        );
    }
} // namespace be::tests::unit
