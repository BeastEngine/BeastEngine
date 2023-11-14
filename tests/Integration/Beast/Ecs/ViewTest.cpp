#include <Beast/Ecs/World.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/Components/Core.h>

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

    struct AddComponentAL : BaseAccessList
    {
        using Add = Components<TestComponent1, TestComponent2, Transform>;
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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity2, {});

        const auto entity3 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity3, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent2>(entity2, {});

        const auto entity3 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity3, {});
        creatorView.AddComponent<TestComponent2>(entity3, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity2, {});

        const auto entity3 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity3, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent2>(entity2, {});

        const auto entity3 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity3, {});
        creatorView.AddComponent<TestComponent2>(entity3, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity2, {});

        const auto entity3 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity3, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity1, {});

        const auto entity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent2>(entity2, {});

        const auto entity3 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(entity3, {});
        creatorView.AddComponent<TestComponent2>(entity3, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto excludedEntity = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(excludedEntity, {});

        const auto includedEntity = creatorView.CreateEntity();
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
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto excludedEntity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(excludedEntity1, {});

        const auto excludedEntity2 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent2>(excludedEntity2, {});

        const auto includedEntity = creatorView.CreateEntity();
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
    static void RunAddCombined()
    {
        std::size_t expectedEntitiesCount = 1;

        World world;
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto excludedEntity1 = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent1>(excludedEntity1, {});

        const auto excludedEntity2 = creatorView.CreateEntity();

        const auto includedEntity = creatorView.CreateEntity();
        creatorView.AddComponent<TestComponent2>(includedEntity, {});

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
        const auto creatorView = world.CreateView<AddComponentAL>();

        std::vector<be::Entity> entities(expectedSize);
        std::generate(entities.begin(), entities.end(), [&creatorView]() {
            auto entity = creatorView.CreateEntity();
            creatorView.AddComponent<TestComponent1>(entity, {});

            return entity;
        });

        [[maybe_unused]] const auto excludedEntity = creatorView.CreateEntity();

        auto sut = world.CreateView<AL>();
        ASSERT_EQ(expectedSize, sut.EntitiesCount());
    }

    TEST_F(ViewTest, CreateEntityWillAddNewEntityToTheWorld)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        World world;
        const auto creatorView = world.CreateView<AddComponentAL>();

        const auto entitiesCountBefore = creatorView.EntitiesCount();
        creatorView.CreateEntity();

        auto sut = world.CreateView<AL>();

        const auto entitiesCountAfter = sut.EntitiesCount();
        ASSERT_NE(entitiesCountBefore, entitiesCountAfter);
        ASSERT_EQ(1, entitiesCountAfter);
    }

    TEST_F(ViewTest, CreateEntityWillAssignTransformComponent)
    {
        struct AssignAL : BaseAccessList
        {
            using Add = Components<Transform>;
        };

        struct GetAL : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        const std::size_t expectedEntitiesCount = 1;

        World world;

        auto getViewBefore = world.CreateView<GetAL>();
        const auto countBefore = getViewBefore.EntitiesCount();

        auto createView = world.CreateView<AssignAL>();
        const auto createdEntity = createView.CreateEntity();

        auto getViewAfter = world.CreateView<GetAL>();
        const auto countAfter = getViewAfter.EntitiesCount();

        const auto& actualTransform = getViewAfter.GetComponent<Transform>(createdEntity);

        ASSERT_EQ(0, countBefore);
        ASSERT_EQ(expectedEntitiesCount, countAfter);
        ASSERT_EQ(Transform{}, actualTransform);
    }

    TEST_F(ViewTest, CreateEntityWillAssignGivenTransformComponent)
    {
        struct AssignAL : BaseAccessList
        {
            using Add = Components<Transform>;
        };

        struct GetAL : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        Transform expectedTransform{
            .position = {7654.0f, -123143.f},
        };

        World world;

        auto createView = world.CreateView<AssignAL>();
        const auto createdEntity = createView.CreateEntity(expectedTransform);

        auto getViewAfter = world.CreateView<GetAL>();
        const auto& actualTransform = getViewAfter.GetComponent<Transform>(createdEntity);

        ASSERT_EQ(expectedTransform, actualTransform);
    }

    TEST_F(ViewTest, EntitiesCountWillReturnNumberOfEntitiesForSingleComponent)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        const auto expectedSize = 5;
        World world;

        const auto creatorView = world.CreateView();
        for (std::size_t i = 0; i < expectedSize; ++i)
        {
            creatorView.CreateEntity();
        }

        const auto sut = world.CreateView<AL>();
        ASSERT_EQ(expectedSize, sut.EntitiesCount());
    }

    TEST_F(ViewTest, EntitiesCountWillReturnNumberOfEntitiesWithAllComponentsAssigned)
    {
        struct CreatorAL : BaseAccessList
        {
            using Add = Components<Transform, TestComponent1, TestComponent2>;
        };

        struct GetterAL : BaseAccessList
        {
            using Get = Components<Transform, TestComponent1, TestComponent2>;
        };

        const auto expectedSize = 5;
        World world;

        const auto creatorView = world.CreateView<CreatorAL>();
        for (std::size_t i = 0; i < expectedSize; ++i)
        {
            const auto entity = creatorView.CreateEntity();
            creatorView.AddComponent(entity, TestComponent1{});
            creatorView.AddComponent(entity, TestComponent2{});
        }

        const auto sut = world.CreateView<GetterAL>();
        ASSERT_EQ(expectedSize, sut.EntitiesCount());
    }

    TEST_F(ViewTest, EntitiesCountWillReturnNumberOfOnlyTheEntitiesThatHaveAllComponentsAssigned)
    {
        struct CreatorAL : BaseAccessList
        {
            using Add = Components<Transform, TestComponent1, TestComponent2>;
        };

        struct GetterAL : BaseAccessList
        {
            using Get = Components<Transform, TestComponent1, TestComponent2>;
        };

        const auto expectedSize = 1;
        World world;

        const auto creatorView = world.CreateView<CreatorAL>();

        creatorView.CreateEntity();
        creatorView.CreateEntity();

        auto entity = creatorView.CreateEntity();
        creatorView.AddComponent(entity, TestComponent1{});

        entity = creatorView.CreateEntity();
        creatorView.AddComponent(entity, TestComponent2{});

        // Only this one has all three components. Transform is assigned automatically.
        entity = creatorView.CreateEntity();
        creatorView.AddComponent(entity, TestComponent1{});
        creatorView.AddComponent(entity, TestComponent2{});

        const auto sut = world.CreateView<GetterAL>();
        ASSERT_EQ(expectedSize, sut.EntitiesCount());
    }
    // TODO: Add proper tests for checking the size of the view in different scenarios
}; // namespace be::tests::integration