#include <Beast/Common/Utils/String.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class ToStringTest : public testing::Test
    {
    };

    TEST_F(ToStringTest, WillConvertGivenIdToString)
    {
        const be::Id id{123456};
        const auto expectedString = "123456";

        const auto actualString = be::ToString(id);
        ASSERT_EQ(expectedString, actualString);
    }
} // namespace be::tests::unit
