#include <Beast/Common/IdAllocatorsFactory.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class IdAllocatorsFactoryTest : public testing::Test
    {
    };

    TEST_F(IdAllocatorsFactoryTest, TryGetWillReturnTheSameAllocatorForTheSameId)
    {
        IdAllocatorsFactory sut{};

        IdAllocator* allocator1 = sut.TryGet(IdAllocator::ID::ID_0);
        ASSERT_NE(allocator1, nullptr);

        const auto generatedId = allocator1->Allocate();
        ASSERT_TRUE(allocator1->IsValid(generatedId));

        IdAllocator* allocator2 = sut.TryGet(IdAllocator::ID::ID_0);
        ASSERT_NE(allocator2, nullptr);
        ASSERT_TRUE(allocator2->IsValid(generatedId));
    }

    TEST_F(IdAllocatorsFactoryTest, TryGetWillReturnDifferentAllocatorsForDifferentId)
    {
        IdAllocatorsFactory sut{};

        IdAllocator* allocator1 = sut.TryGet(IdAllocator::ID::ID_0);
        const auto generatedId = allocator1->Allocate();
        ASSERT_NE(allocator1, nullptr);
        ASSERT_TRUE(allocator1->IsValid(generatedId));

        IdAllocator* allocator2 = sut.TryGet(IdAllocator::ID::ID_1);
        ASSERT_NE(allocator2, nullptr);
        ASSERT_FALSE(allocator2->IsValid(generatedId));
    }

    TEST_F(IdAllocatorsFactoryTest, TryGetWillReturnAllocatorForIdsFromAvailableRange)
    {
        IdAllocatorsFactory sut{};

        const auto CanGetAllocator = [&](IdAllocator::ID allocatorId) {
            IdAllocator* alloc = sut.TryGet(allocatorId);
            return alloc != nullptr;
        };
        
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_0));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_1));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_2));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_3));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_4));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_5));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_6));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_7));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_8));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_9));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_10));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_11));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_12));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_13));
        ASSERT_TRUE(CanGetAllocator(IdAllocator::ID::ID_LAST));
    }
} // namespace be::tests::unit
