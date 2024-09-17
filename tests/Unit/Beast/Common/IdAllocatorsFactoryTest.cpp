#include <Beast/Common/IdAllocatorsFactory.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class IdAllocatorsFactoryTest : public testing::Test
    {
    };

    TEST_F(IdAllocatorsFactoryTest, GetWillReturnTheSameAllocatorForTheSameId)
    {
        IdAllocatorsFactory sut{};

        IdAllocator& allocator1 = sut.Get(IdAllocator::ID::ID_0);
        const auto generatedId = allocator1.Allocate();
        ASSERT_TRUE(allocator1.IsValid(generatedId));

        IdAllocator& allocator2 = sut.Get(IdAllocator::ID::ID_0);
        ASSERT_TRUE(allocator2.IsValid(generatedId));
    }

    TEST_F(IdAllocatorsFactoryTest, GetWillReturnDifferentAllocatorsForDifferentId)
    {
        IdAllocatorsFactory sut{};

        IdAllocator& allocator1 = sut.Get(IdAllocator::ID::ID_0);
        const auto generatedId = allocator1.Allocate();
        ASSERT_TRUE(allocator1.IsValid(generatedId));

        IdAllocator& allocator2 = sut.Get(IdAllocator::ID::ID_1);
        ASSERT_FALSE(allocator2.IsValid(generatedId));
    }

    TEST_F(IdAllocatorsFactoryTest, GetWillReturnAllocatorForIdsFromAvailableRange)
    {
        IdAllocatorsFactory sut{};
        
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_0));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_1));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_2));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_3));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_4));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_5));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_6));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_7));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_8));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_9));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_10));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_11));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_12));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_13));
        ASSERT_NO_THROW([[maybe_unused]] auto& alloc = sut.Get(IdAllocator::ID::ID_LAST));
    }
} // namespace be::tests::unit
