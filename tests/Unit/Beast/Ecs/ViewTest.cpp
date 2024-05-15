#include <Beast/Ecs/World.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/Components/Core.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    struct TestComponent1
    {
        int data = 0;
    };

    struct TestComponent2
    {
        const char* data = "TestComponent2";
    };

    struct TestTag1
    {
    };

    struct TestTag2
    {
    };

    struct AddComponentAL : BaseAccessList
    {
        using Add = Components<TestComponent1, TestComponent2, Transform>;
    };

    struct AddTagAL : BaseAccessList
    {
        using Add = Components<TestTag1, TestTag2, Transform>;
    };

    // TODO: Add tests that check if RemoveComponent and RemoveTag actually remove anything!

    namespace view_components
    {
        class ViewTestComponents : public testing::Test
        {
        };

        TEST_F(ViewTestComponents, IterationWillIncludeAllEntitiesWithComponentFromGetAccessList)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestComponent1>;
            };

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillIncludeAllEntitiesWithConjuctionOfComponentsFromGetAccessList)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestComponent1, TestComponent2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillIncludeAllEntitiesWithComponentFromUpdateAccessList)
        {
            struct AL : BaseAccessList
            {
                using Update = Components<TestComponent1>;
            };

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillIncludeAllEntitiesWithConjuctionOfComponentsFromUpdateAccessList)
        {
            struct AL : BaseAccessList
            {
                using Update = Components<TestComponent1, TestComponent2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillIncludeAllEntitiesWithComponentFromRemoveAccessList)
        {
            struct AL : BaseAccessList
            {
                using Remove = Components<TestComponent1>;
            };

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillIncludeAllEntitiesWithConjuctionOfComponentsFromRemoveAccessList)
        {
            struct AL : BaseAccessList
            {
                using Remove = Components<TestComponent1, TestComponent2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillExcludeEntitiesWithComponentFromAddAccessList)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestComponent1>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillExcludeEntitiesWithAnyOfTheComponentsFromAddAccessList)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestComponent1, TestComponent2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
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

            World world{};
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

        TEST_F(ViewTestComponents, IterationWillCombineEntitiesForAddAndGetComponents)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestComponent1>;
                using Get = Components<TestComponent2>;
            };

            RunAddCombined<AL>();
        }

        TEST_F(ViewTestComponents, IterationWillCombineEntitiesForAddAndUpdateComponents)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestComponent1>;
                using Update = Components<TestComponent2>;
            };

            RunAddCombined<AL>();
        }

        TEST_F(ViewTestComponents, IterationWillCombineEntitiesForAddAndRemoveComponents)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestComponent1>;
                using Remove = Components<TestComponent2>;
            };

            RunAddCombined<AL>();
        }

        TEST_F(ViewTestComponents, SizeWillReturnNumberOfEntitiesInTheView)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestComponent1>;
            };

            const std::size_t expectedSize = 5;

            World world{};
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

        TEST_F(ViewTestComponents, CreateEntityWillAddNewEntityToTheWorld)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<Transform>;
            };

            World world{};
            const auto creatorView = world.CreateView<AddComponentAL>();

            const auto entitiesCountBefore = creatorView.EntitiesCount();
            creatorView.CreateEntity();

            auto sut = world.CreateView<AL>();

            const auto entitiesCountAfter = sut.EntitiesCount();
            ASSERT_NE(entitiesCountBefore, entitiesCountAfter);
            ASSERT_EQ(1, entitiesCountAfter);
        }

        TEST_F(ViewTestComponents, CreateEntityWillAssignTransformComponent)
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

            World world{};

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

        TEST_F(ViewTestComponents, CreateEntityWillAssignGivenTransformComponent)
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

            World world{};

            auto createView = world.CreateView<AssignAL>();
            const auto createdEntity = createView.CreateEntity(expectedTransform);

            auto getViewAfter = world.CreateView<GetAL>();
            const auto& actualTransform = getViewAfter.GetComponent<Transform>(createdEntity);

            ASSERT_EQ(expectedTransform, actualTransform);
        }

        TEST_F(ViewTestComponents, EntitiesCountWillReturnNumberOfEntitiesForSingleComponent)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<Transform>;
            };

            const auto expectedSize = 5;
            World world{};

            const auto creatorView = world.CreateView();
            for (std::size_t i = 0; i < expectedSize; ++i)
            {
                creatorView.CreateEntity();
            }

            const auto sut = world.CreateView<AL>();
            ASSERT_EQ(expectedSize, sut.EntitiesCount());
        }

        TEST_F(ViewTestComponents, EntitiesCountWillReturnNumberOfEntitiesWithAllComponentsAssigned)
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
            World world{};

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

        TEST_F(ViewTestComponents, EntitiesCountWillReturnNumberOfOnlyTheEntitiesThatHaveAllComponentsAssigned)
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
            World world{};

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

        TEST_F(ViewTestComponents, EntitiesCountWillReturnNumberOfEntitiesWithGivenComponent)
        {
            struct CreatorAL : BaseAccessList
            {
                using Add = Components<Transform, TestComponent1>;
            };

            World world{};
            const auto view = world.CreateView<CreatorAL>();

            ASSERT_EQ(0, view.EntitiesCount<TestComponent1>());

            {
                const auto entity = view.CreateEntity();
                view.AddComponent(entity, TestComponent1{});
            }
            ASSERT_EQ(1, view.EntitiesCount<TestComponent1>());

            {
                const auto entity = view.CreateEntity();
                view.AddComponent(entity, TestComponent1{});
            }
            ASSERT_EQ(2, view.EntitiesCount<TestComponent1>());
        }
    } // namespace view_components

    namespace view_tags
    {
        class ViewTestTags : public testing::Test
        {
        };

        TEST_F(ViewTestTags, IterationWillIncludeAllEntitiesWithTagFromGetAccessList)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestTag1>;
            };

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto entity1 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity1);

            const auto entity2 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity2);

            const auto entity3 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity3);

            std::vector<be::Entity> expectedEntities{entity1, entity2, entity3};
            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_TRUE(std::ranges::find(expectedEntities, entity) != expectedEntities.end()) << "View contains additional entity: " << ToIntegral(entity);
                ASSERT_TRUE(sut.HasTag<TestTag1>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntities.size(), actualEntitiesCount);
        }

        TEST_F(ViewTestTags, IterationWillIncludeAllEntitiesWithConjuctionOfTagsFromGetAccessList)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestTag1, TestTag2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto entity1 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity1);

            const auto entity2 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag2>(entity2);

            const auto entity3 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity3);
            creatorView.AddTag<TestTag2>(entity3);

            const auto expectedEntity = entity3;
            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_EQ(expectedEntity, entity);
                ASSERT_TRUE(sut.HasTag<TestTag1>(entity));
                ASSERT_TRUE(sut.HasTag<TestTag2>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
        }

        TEST_F(ViewTestTags, IterationWillIncludeAllEntitiesWithTagFromRemoveAccessList)
        {
            struct AL : BaseAccessList
            {
                using Remove = Components<TestTag1>;
            };

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto entity1 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity1);

            const auto entity2 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity2);

            const auto entity3 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity3);

            std::vector<be::Entity> expectedEntities{entity1, entity2, entity3};
            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_TRUE(std::ranges::find(expectedEntities, entity) != expectedEntities.end()) << "View contains additional entity: " << ToIntegral(entity);
                ASSERT_NO_THROW(sut.RemoveTag<TestTag1>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntities.size(), actualEntitiesCount);
            ASSERT_EQ(0, sut.EntitiesCount());
        }

        TEST_F(ViewTestTags, IterationWillIncludeAllEntitiesWithConjuctionOfTagsFromRemoveAccessList)
        {
            struct AL : BaseAccessList
            {
                using Remove = Components<TestTag1, TestTag2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto entity1 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity1);

            const auto entity2 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag2>(entity2);

            const auto entity3 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity3);
            creatorView.AddTag<TestTag2>(entity3);

            const auto expectedEntity = entity3;
            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_EQ(expectedEntity, entity);
                ASSERT_NO_THROW(sut.RemoveTag<TestTag1>(entity));
                ASSERT_NO_THROW(sut.RemoveTag<TestTag2>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
            ASSERT_EQ(0, sut.EntitiesCount());
        }

        TEST_F(ViewTestTags, IterationWillExcludeEntitiesWithTagFromAddAccessList)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestTag1>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto excludedEntity = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(excludedEntity);

            const auto includedEntity = creatorView.CreateEntity();
            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_NE(excludedEntity, entity);

                ASSERT_EQ(includedEntity, entity);
                ASSERT_NO_THROW(sut.AddTag<TestTag1>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
        }

        TEST_F(ViewTestTags, IterationWillExcludeEntitiesWithAnyOfTheTagsFromAddAccessList)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestTag1, TestTag2>;
            };

            std::size_t expectedEntitiesCount = 1;

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto excludedEntity1 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(excludedEntity1);

            const auto excludedEntity2 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag2>(excludedEntity2);

            const auto includedEntity = creatorView.CreateEntity();
            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_NE(excludedEntity1, entity);
                ASSERT_NE(excludedEntity2, entity);

                ASSERT_EQ(includedEntity, entity);
                ASSERT_NO_THROW(sut.AddTag<TestTag1>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
        }

        template<typename AL>
        static void RunAddCombined()
        {
            std::size_t expectedEntitiesCount = 1;

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            const auto excludedEntity1 = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(excludedEntity1);

            const auto excludedEntity2 = creatorView.CreateEntity();

            const auto includedEntity = creatorView.CreateEntity();
            creatorView.AddTag<TestTag2>(includedEntity);

            std::size_t actualEntitiesCount = 0;

            const auto sut = world.CreateView<AL>();
            for (const auto& entity : sut)
            {
                ASSERT_NE(excludedEntity1, entity);
                ASSERT_NE(excludedEntity2, entity);

                ASSERT_EQ(includedEntity, entity);
                ASSERT_NO_THROW(sut.AddTag<TestTag1>(entity));

                ++actualEntitiesCount;
            }

            ASSERT_EQ(expectedEntitiesCount, actualEntitiesCount);
        }

        TEST_F(ViewTestTags, IterationWillCombineEntitiesForAddAndGetTags)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestTag1>;
                using Get = Components<TestTag2>;
            };

            RunAddCombined<AL>();
        }

        TEST_F(ViewTestTags, IterationWillCombineEntitiesForAddAndUpdateTags)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestTag1>;
                using Update = Components<TestTag2>;
            };

            RunAddCombined<AL>();
        }

        TEST_F(ViewTestTags, IterationWillCombineEntitiesForAddAndRemoveTags)
        {
            struct AL : BaseAccessList
            {
                using Add = Components<TestTag1>;
                using Remove = Components<TestTag2>;
            };

            RunAddCombined<AL>();
        }

        TEST_F(ViewTestTags, SizeWillReturnNumberOfEntitiesInTheView)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestTag1>;
            };

            const std::size_t expectedSize = 5;

            World world{};
            const auto creatorView = world.CreateView<AddTagAL>();

            std::vector<be::Entity> entities(expectedSize);
            std::generate(entities.begin(), entities.end(), [&creatorView]() {
                auto entity = creatorView.CreateEntity();
                creatorView.AddTag<TestTag1>(entity);

                return entity;
            });

            [[maybe_unused]] const auto excludedEntity = creatorView.CreateEntity();

            auto sut = world.CreateView<AL>();
            ASSERT_EQ(expectedSize, sut.EntitiesCount());
        }

        TEST_F(ViewTestTags, EntitiesCountWillReturnNumberOfEntitiesForSingleTag)
        {
            struct AL : BaseAccessList
            {
                using Get = Components<TestTag1>;
            };

            const auto expectedSize = 5;
            World world{};

            const auto creatorView = world.CreateView<AddTagAL>();
            for (std::size_t i = 0; i < expectedSize; ++i)
            {
                const auto entity = creatorView.CreateEntity();
                creatorView.AddTag<TestTag1>(entity);
            }

            const auto sut = world.CreateView<AL>();
            ASSERT_EQ(expectedSize, sut.EntitiesCount());
        }

        TEST_F(ViewTestTags, EntitiesCountWillReturnNumberOfEntitiesWithAllTagsAssigned)
        {
            struct CreatorAL : BaseAccessList
            {
                using Add = Components<Transform, TestTag1, TestTag2>;
            };

            struct GetterAL : BaseAccessList
            {
                using Get = Components<Transform, TestTag1, TestTag2>;
            };

            const auto expectedSize = 5;
            World world{};

            const auto creatorView = world.CreateView<CreatorAL>();
            for (std::size_t i = 0; i < expectedSize; ++i)
            {
                const auto entity = creatorView.CreateEntity();
                creatorView.AddTag<TestTag1>(entity);
                creatorView.AddTag<TestTag2>(entity);
            }

            const auto sut = world.CreateView<GetterAL>();
            ASSERT_EQ(expectedSize, sut.EntitiesCount());
        }

        TEST_F(ViewTestTags, EntitiesCountWillReturnNumberOfOnlyTheEntitiesThatHaveAllTagsAssigned)
        {
            struct CreatorAL : BaseAccessList
            {
                using Add = Components<Transform, TestTag1, TestTag2>;
            };

            struct GetterAL : BaseAccessList
            {
                using Get = Components<Transform, TestTag1, TestTag2>;
            };

            const auto expectedSize = 1;
            World world{};

            const auto creatorView = world.CreateView<CreatorAL>();

            creatorView.CreateEntity();
            creatorView.CreateEntity();

            auto entity = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity);

            entity = creatorView.CreateEntity();
            creatorView.AddTag<TestTag2>(entity);

            // Only this one has all three components. Transform is assigned automatically.
            entity = creatorView.CreateEntity();
            creatorView.AddTag<TestTag1>(entity);
            creatorView.AddTag<TestTag2>(entity);

            const auto sut = world.CreateView<GetterAL>();
            ASSERT_EQ(expectedSize, sut.EntitiesCount());
        }

        TEST_F(ViewTestTags, EntitiesCountWillReturnNumberOfEntitiesWithGivenTag)
        {
            struct CreatorAL : BaseAccessList
            {
                using Add = Components<Transform, TestTag1>;
            };

            World world{};
            const auto view = world.CreateView<CreatorAL>();

            ASSERT_EQ(0, view.EntitiesCount<TestTag1>());
            {
                const auto entity = view.CreateEntity();
                view.AddTag<TestTag1>(entity);
            }
            ASSERT_EQ(1, view.EntitiesCount<TestTag1>());
            {
                const auto entity = view.CreateEntity();
                view.AddTag<TestTag1>(entity);
            }
            ASSERT_EQ(2, view.EntitiesCount<TestTag1>());
        }
    } // namespace view_tags
}; // namespace be::tests::unit