#include <Beast/Ecs/World.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/Types.h>

#include <gtest/gtest.h>

namespace be::tests::integration
{
    class WorldTest : public testing::Test
    {
    };

    struct TestComponent
    {
        int data = 10;
    };

    TEST_F(WorldTest, CreateViewWillCreateViewWithSpecifiedGetAccessList)
    {
        struct AL : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        World sut;
        const auto entity = sut.CreateView().CreateEntity();

        const auto view = sut.CreateView<AL>();

        const Transform expectedComponent{};
        const auto& actualComponent = view.GetComponent<Transform>(entity);

        ASSERT_EQ(expectedComponent.position, actualComponent.position);
    }

    TEST_F(WorldTest, CreateViewWillCreateViewWithSpecifiedUpdateAccessList)
    {
        struct AL : BaseAccessList
        {
            using Update = Components<Transform>;
        };

        World sut;
        const auto entity = sut.CreateView().CreateEntity();

        const auto view = sut.CreateView<AL>();

        auto& firstComponent = view.UpdateComponent<Transform>(entity);
        firstComponent.position.x = 5;

        auto& secondCompoent = view.UpdateComponent<Transform>(entity);
        ASSERT_EQ(firstComponent.position, secondCompoent.position);
    }

    TEST_F(WorldTest, CreateViewWillCreateViewWithSpecifiedAddAccessList)
    {
        struct TestComponent
        {
            int data = 10;
        };

        struct TestedAL : BaseAccessList
        {
            using Add = Components<TestComponent>;
        };

        const int expectedData = 15;

        World sut;
        const auto entity = sut.CreateView().CreateEntity();

        const auto view = sut.CreateView<TestedAL>();
        view.AddComponent<TestComponent>(entity, {.data = expectedData});

        struct AL : BaseAccessList
        {
            using Get = Components<TestComponent>;
        };

        const auto secondView = sut.CreateView<AL>();
        const auto& component = secondView.GetComponent<TestComponent>(entity);
        ASSERT_EQ(expectedData, component.data);
    }

    TEST_F(WorldTest, CreateViewWillCreateViewWithSpecifiedRemoveAccessList)
    {
        struct AL : BaseAccessList
        {
            using Remove = Components<Transform>;
        };

        World sut;
        const auto entity = sut.CreateView().CreateEntity();

        const auto view = sut.CreateView<AL>();
        ASSERT_TRUE(view.HasComponent<Transform>(entity));

        view.RemoveComponent<Transform>(entity);
        ASSERT_FALSE(view.HasComponent<Transform>(entity));
    }

    TEST_F(WorldTest, CreateViewWithDefaultParameterWillReturnViewCapableOfCreatingEntities)
    {
        struct GetTransformAL : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        World sut;
        const auto verifierView = sut.CreateView<GetTransformAL>();
        ASSERT_EQ(0, verifierView.EntitiesCount());

        const auto view = sut.CreateView();
        const auto entity = view.CreateEntity();

        ASSERT_EQ(1, verifierView.EntitiesCount());
        ASSERT_TRUE(verifierView.HasComponent<Transform>(entity));
    }
} // namespace be::tests::integration
