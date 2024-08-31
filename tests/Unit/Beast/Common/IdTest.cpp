#include <Beast/Common/Id.h>

#include <gtest/gtest.h>
#include <unordered_set>

namespace be::tests::unit
{
    class IdTest : public testing::Test
    {
    };

    TEST_F(IdTest, ToStringWillConvertGivenIdToString)
    {
        const be::Id id{123456};
        const auto expectedString = "123456";

        const auto actualString = id.ToString();
        ASSERT_EQ(expectedString, actualString);
    }

    TEST_F(IdTest, StringConstructorWillGenerateIdFromStringHash)
    {
        const std::string_view stringValue = "123456";
        const auto expectedHash = be::Hash(stringValue);
        const be::Id expectedId(expectedHash);

        const auto actualId = Id(stringValue);
        ASSERT_EQ(expectedId, actualId);
    }
} // namespace be::tests::unit
