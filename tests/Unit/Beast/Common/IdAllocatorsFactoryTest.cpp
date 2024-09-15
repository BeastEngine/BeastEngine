#include <Beast/Common/IdAllocatorsFactory.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class IdAllocatorsFactoryTest : public testing::Test
    {
    };

    TEST_F(IdAllocatorsFactoryTest, GetWillReturnTheSameAllocatorForTheSameId)
    {
        IdAllocatorsFactory sut{AAID_ALLOCATOR_MASK};

        AllocatorIndex index{0};
        
        IdAllocator& allocator1 = sut.Get(index);
        const auto generatedId = allocator1.Allocate();
        ASSERT_TRUE(allocator1.IsValid(generatedId));

        IdAllocator& allocator2 = sut.Get(index);
        ASSERT_TRUE(allocator2.IsValid(generatedId));
    }
} // namespace be::tests::unit
