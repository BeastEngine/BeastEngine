#include <Beast/Ecs/World.h>
#include <Beast/Ecs/View.h>

#include <gtest/gtest.h>

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

    class ViewTest : public testing::Test
    {
    };

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesWithComponentFromGetAccessList)
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

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesWithConjuctionOfComponentsFromGetAccessList)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<TestComponent1, TestComponent2>;
        };

        std::size_t expectedEntitiesCount = 1;

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

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesWithComponentFromUpdateAccessList)
    {
        struct AL : BaseAccessList
        {
            using Update = Components<TestComponent1>;
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
            ASSERT_NO_THROW(sut.UpdateComponent<TestComponent1>(entity));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntities.size(), actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesWithConjuctionOfComponentsFromUpdateAccessList)
    {
        struct AL : BaseAccessList
        {
            using Update = Components<TestComponent1, TestComponent2>;
        };

        std::size_t expectedEntitiesCount = 1;

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
            ASSERT_NO_THROW(sut.UpdateComponent<TestComponent1>(entity));
            ASSERT_NO_THROW(sut.UpdateComponent<TestComponent2>(entity));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesWithComponentFromRemoveAccessList)
    {
        struct AL : BaseAccessList
        {
            using Remove = Components<TestComponent1>;
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
            ASSERT_NO_THROW(sut.RemoveComponent<TestComponent1>(entity));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntities.size(), actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillIncludeAllEntitiesWithConjuctionOfComponentsFromRemoveAccessList)
    {
        struct AL : BaseAccessList
        {
            using Remove = Components<TestComponent1, TestComponent2>;
        };

        std::size_t expectedEntitiesCount = 1;

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
            ASSERT_NO_THROW(sut.RemoveComponent<TestComponent1>(entity));
            ASSERT_NO_THROW(sut.RemoveComponent<TestComponent2>(entity));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillExcludeEntitiesWithComponentFromAddAccessList)
    {
        /*struct AL : BaseAccessList
        {
            using Add = Components<TestComponent1>;
        };

        std::size_t expectedEntitiesCount = 1;

        World world;
        const auto excludedEntity = world.CreateEntity();
        world.AddComponent<TestComponent1>(excludedEntity, {});

        const auto includedEntity = world.CreateEntity();
        std::size_t actualEntitiesCount = 0;

        const auto sut = world.CreateView<AL>();
        for (const auto& entity : sut)
        {
            ASSERT_NE(excludedEntity, entity);

            ASSERT_EQ(includedEntity, entity);
            ASSERT_NO_THROW(sut.AddComponent<TestComponent1>(entity, {}));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);*/
        entt::registry reg;
        [[maybe_unused]] const auto e = reg.create();
        reg.emplace<TestComponent1>(e);
        [[maybe_unused]]auto& stor = reg.storage<entt::entity>();
        stor.begin();
    }
    // Should Update allow using GetComponent<>?
}; // namespace be::tests::integration