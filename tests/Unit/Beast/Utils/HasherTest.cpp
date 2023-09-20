#include <Beast/Common/Utils/Hasher.h>
#include <gtest/gtest.h>

#include <array>
#include <string_view>

namespace be::tests::unit
{
    class HasherTest : public testing::Test
    {
    };

    TEST_F(HasherTest, HashWillReturnTheSameHashForTheSameInput)
    {
        std::array<std::string_view, 3> inputs{
            "string1",
            "string2",
            "C:/Path/To/My/Texture.png",
        };

        for (const auto& input : inputs)
        {
            const auto firstHash = be::Hash(input);
            const auto secondHash = be::Hash(input);

            ASSERT_EQ(firstHash, secondHash) << "Hashes for '" << input << "' input don't match!\n"; 
        }
    }
} // namespace be::tests::unit