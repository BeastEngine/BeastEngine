#include <Integration/Beast/Ecs/ViewTest.h>

#include <Beast/Ecs/World.h>
#include <Beast/Ecs/View.h>

namespace be::tests::integration
{
    struct TestComponent1
    {
        int data = 0;
    };

    struct TestComponent2
    {
        const char* data = "TestComponent2";
    };

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesFromGetAccessList)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<TestComponent1>;
        };

        World world;
        const auto entity1 = world.CreateEntity();
        world.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = world.CreateEntity();
        world.AddComponent<TestComponent1>(entity2, {});

        const auto entity3 = world.CreateEntity();
        world.AddComponent<TestComponent1>(entity3, {});

        std::vector<be::Entity> expectedEntities{entity1, entity2, entity3};
        std::size_t actualEntitiesCount = 0;

        const auto sut = world.CreateView<AL>();
        for (const auto& entity : sut)
        {
            ASSERT_TRUE(std::ranges::find(expectedEntities, entity) != expectedEntities.end()) << "View contains additional entity: " << ToIntegral(entity);
            ASSERT_NO_THROW(sut.GetComponent<TestComponent1>(entity));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntities.size(), actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillIncludeConjuctionOfAllEntitiesFromGetAccessList)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<TestComponent1, TestComponent2>;
        };

        World world;
        const auto entity1 = world.CreateEntity();
        world.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = world.CreateEntity();
        world.AddComponent<TestComponent2>(entity2, {});

        const auto entity3 = world.CreateEntity();
        world.AddComponent<TestComponent1>(entity3, {});
        world.AddComponent<TestComponent2>(entity3, {});

        const auto expectedEntity = entity3;
        std::size_t actualEntitiesCount = 0;

        const auto sut = world.CreateView<AL>();
        for (const auto& entity : sut)
        {
            ASSERT_EQ(expectedEntity, entity);
            ASSERT_NO_THROW(sut.GetComponent<TestComponent1>(entity));
            ASSERT_NO_THROW(sut.GetComponent<TestComponent2>(entity));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(1, actualEntitiesCount);
    }
}; // namespace be::tests::integration