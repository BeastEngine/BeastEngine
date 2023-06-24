#include <Integration/Beast/Ecs/WorldTest.h>

#include <Beast/Ecs/World.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/Types.h>

namespace be::tests::integration
{
    TEST_F(WorldTest, CreateEntityWillAddNewEntityToTheWorld)
    {
        World sut;
        const auto newEntity = sut.CreateEntity();

        ASSERT_TRUE(newEntity != NULL_ENTITY);
        ASSERT_TRUE(sut.IsValid(newEntity));
    }

    struct TestComponent
    {
        int data = 10;
    };

    TEST_F(WorldTest, CreateViewWillCreateViewWithSpecifiedGetAccessList)
    {
        struct TestComponent
        {
            int data = 10;
        };

        struct AL : BaseAccessList
        {
            using Get = Components<TestComponent>;
        };

        World sut;
        const auto entity = sut.CreateEntity();
        sut.AddComponent<TestComponent>(entity, {});

        const auto view = sut.CreateView<AL>();

        const TestComponent expectedComponent{};
        const auto& actualComponent = view.GetComponent<TestComponent>(entity);
        ASSERT_EQ(expectedComponent.data, actualComponent.data);
    }

    TEST_F(WorldTest, CreateViewWillCreateViewWithSpecifiedUpdateAccessList)
    {
        struct TestComponent
        {
            int data = 10;
        };

        struct AL : BaseAccessList
        {
            using Update = Components<TestComponent>;
        };

        World sut;
        const auto entity = sut.CreateEntity();
        sut.AddComponent<TestComponent>(entity, {});

        const auto view = sut.CreateView<AL>();

        auto& firstComponent = view.UpdateComponent<TestComponent>(entity);
        firstComponent.data = 5;

        auto& secondCompoent = view.UpdateComponent<TestComponent>(entity);
        ASSERT_EQ(firstComponent.data, secondCompoent.data);
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
        const auto entity = sut.CreateEntity();

        const auto view = sut.CreateView<TestedAL>();
        view.AddComponent<TestComponent>(entity, {.data=expectedData});

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
        struct TestComponent
        {
            int data = 10;
        };

        struct AL : BaseAccessList
        {
            using Remove = Components<TestComponent>;
        };

        World sut;
        const auto entity = sut.CreateEntity();
        sut.AddComponent<TestComponent>(entity, {});

        const auto view = sut.CreateView<AL>();
        ASSERT_TRUE(view.HasComponent<TestComponent>(entity));
        
        view.RemoveComponent<TestComponent>(entity);
        ASSERT_FALSE(view.HasComponent<TestComponent>(entity));
    }
}