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
        struct AL : BaseAccessList
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

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillExcludeEntitiesWithAnyOfTheComponentsFromAddAccessList)
    {
        struct AL : BaseAccessList
        {
            using Add = Components<TestComponent1, TestComponent2>;
        };

        std::size_t expectedEntitiesCount = 1;

        World world;
        const auto excludedEntity1 = world.CreateEntity();
        world.AddComponent<TestComponent1>(excludedEntity1, {});

        const auto excludedEntity2 = world.CreateEntity();
        world.AddComponent<TestComponent2>(excludedEntity2, {});

        const auto includedEntity = world.CreateEntity();
        std::size_t actualEntitiesCount = 0;

        const auto sut = world.CreateView<AL>();
        for (const auto& entity : sut)
        {
            ASSERT_NE(excludedEntity1, entity);
            ASSERT_NE(excludedEntity2, entity);

            ASSERT_EQ(includedEntity, entity);
            ASSERT_NO_THROW(sut.AddComponent<TestComponent1>(entity, {}));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
    }

    template<typename AL>
    static constexpr void RunAddCombined()
    {
        std::size_t expectedEntitiesCount = 1;

        World world;
        const auto excludedEntity1 = world.CreateEntity();
        world.AddComponent<TestComponent1>(excludedEntity1, {});

        const auto excludedEntity2 = world.CreateEntity();

        const auto includedEntity = world.CreateEntity();
        world.AddComponent<TestComponent2>(includedEntity, {});

        std::size_t actualEntitiesCount = 0;

        const auto sut = world.CreateView<AL>();
        for (const auto& entity : sut)
        {
            ASSERT_NE(excludedEntity1, entity);
            ASSERT_NE(excludedEntity2, entity);

            ASSERT_EQ(includedEntity, entity);
            ASSERT_NO_THROW(sut.AddComponent<TestComponent1>(entity, {}));

            ++actualEntitiesCount;
        }

        ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
    }

    TEST_F(ViewTest, IterationWillCombineEntitiesForAddAndGetComponents)
    {
        struct AL : BaseAccessList
        {
            using Add = Components<TestComponent1>;
            using Get = Components<TestComponent2>;
        };

        RunAddCombined<AL>();
    }

    TEST_F(ViewTest, IterationWillCombineEntitiesForAddAndUpdateComponents)
    {
        struct AL : BaseAccessList
        {
            using Add = Components<TestComponent1>;
            using Update = Components<TestComponent2>;
        };

        RunAddCombined<AL>();
    }

    TEST_F(ViewTest, IterationWillCombineEntitiesForAddAndRemoveComponents)
    {
        struct AL : BaseAccessList
        {
            using Add = Components<TestComponent1>;
            using Remove = Components<TestComponent2>;
        };

        RunAddCombined<AL>();
    }

    TEST_F(ViewTest, SizeWillReturnNumberOfEntitiesInTheView)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<TestComponent1>;
        };

        const std::size_t expectedSize = 5;

        World world;
        std::vector<be::Entity> entities(expectedSize);
        std::generate(entities.begin(), entities.end(), [&world]() {
            auto entity = world.CreateEntity();
            world.AddComponent<TestComponent1>(entity, {});
            return entity;
        });

        [[maybe_unused]] const auto excludedEntity = world.CreateEntity();

        auto sut = world.CreateView<AL>();
        ASSERT_EQ(expectedSize, sut.Size());
    }
}; // namespace be::tests::integration