#include <Beast/Common/IdAllocator.h>
#include <Beast/Common/Types.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class IdAllocatorTest : public testing::Test
    {
    protected:
        const uint8 allocatorId = 0;
        IdAllocator sut{allocatorId};
    };

    TEST_F(IdAllocatorTest, AllocateWillAllocateNewIdWithIncreasingIndexZeroVersionAndAllocatorId)
    {
        const AllocatorAwareId expectedId1{0};
        const AllocatorAwareId expectedId2{1};
        const AllocatorAwareId expectedId3{2};

        ASSERT_EQ(expectedId1, sut.Allocate());
        ASSERT_EQ(expectedId2, sut.Allocate());
        ASSERT_EQ(expectedId3, sut.Allocate());
    }

    TEST_F(IdAllocatorTest, AllocateWillReusePreviousIndexButWithIncreasedVersion)
    {
        const uint32 expectedIndex = 0;
        const uint16 expectedVersion = 1;

        const AllocatorAwareId expectedFirstId{expectedIndex};
        const AllocatorAwareId expectedReusedId{expectedIndex | (expectedVersion << AAID_VERSION_START_BIT)};

        const auto actualFirstId = sut.Allocate();
        ASSERT_EQ(expectedFirstId, actualFirstId);

        sut.Deallocate(expectedFirstId);
        const auto actualReusedId = sut.Allocate();

        ASSERT_EQ(expectedReusedId, actualReusedId);
    }

    TEST_F(IdAllocatorTest, DeallocateWillThrowIfGivenInvalidId)
    {
        const AllocatorAwareId outOfRangeId{123456};
        const AllocatorAwareId destroyedId = sut.Allocate();
        sut.Deallocate(destroyedId);

        ASSERT_ANY_THROW(sut.Deallocate(outOfRangeId));
        ASSERT_ANY_THROW(sut.Deallocate(destroyedId));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnTrueForValidIds)
    {
        const AllocatorAwareId id1 = sut.Allocate();
        const AllocatorAwareId id2 = sut.Allocate();
        const AllocatorAwareId id3 = sut.Allocate();

        ASSERT_TRUE(sut.IsValid(id1));
        ASSERT_TRUE(sut.IsValid(id2));
        ASSERT_TRUE(sut.IsValid(id3));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnFalseForDeallocatedIds)
    {
        const AllocatorAwareId id = sut.Allocate();
        ASSERT_TRUE(sut.IsValid(id));

        sut.Deallocate(id);
        ASSERT_FALSE(sut.IsValid(id));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnTrueForReusedIds)
    {
        const AllocatorAwareId id = sut.Allocate();
        ASSERT_TRUE(sut.IsValid(id));
        sut.Deallocate(id);

        const AllocatorAwareId reusedId = sut.Allocate();
        ASSERT_NE(reusedId, id);

        ASSERT_TRUE(sut.IsValid(reusedId));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnFalseForIdOutOfRange)
    {
        const AllocatorAwareId outOfRangeId{123456};
        ASSERT_FALSE(sut.IsValid(outOfRangeId));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnFalseForNullId)
    {
        ASSERT_FALSE(sut.IsValid(AAID_NULL));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnFalseForIdFromDifferentAllocator)
    {
        const uint8 allocatorId0 = 0;
        const uint8 allocatorId1 = 1;

        IdAllocator allocator0{allocatorId0};
        IdAllocator allocator1{allocatorId1};

        const auto id0 = allocator0.Allocate();
        const auto id1 = allocator1.Allocate();

        ASSERT_FALSE(allocator0.IsValid(id1));
        ASSERT_FALSE(allocator1.IsValid(id0));
    }

    TEST_F(IdAllocatorTest, IsValidWillReturnFalseForIdWithSameIndexButDifferentVersion)
    {
        const uint32 expectedIndex = 0;
        const uint16 expectedVersion = 1;

        const AllocatorAwareId expectedFirstId{expectedIndex};
        const AllocatorAwareId expectedReusedId{expectedIndex | (expectedVersion << AAID_VERSION_START_BIT)};

        const auto actualFirstId = sut.Allocate();
        ASSERT_EQ(expectedFirstId, actualFirstId);

        sut.Deallocate(expectedFirstId);

        const auto actualReusedId = sut.Allocate();
        ASSERT_EQ(expectedReusedId, actualReusedId);

        ASSERT_FALSE(sut.IsValid(actualFirstId));
        ASSERT_FALSE(sut.IsValid(expectedFirstId));

        ASSERT_TRUE(sut.IsValid(actualReusedId));
        ASSERT_TRUE(sut.IsValid(expectedReusedId));
    }
} // namespace be::tests::unit
