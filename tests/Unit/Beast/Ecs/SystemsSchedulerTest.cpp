#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class SystemsSchedulerGroupTest : public testing::Test
    {
    };

    class SystemsSchedulerTest : public testing::Test
    {
    };

    struct MyComponent
    {
        int value = 0;
    };

    class TestSystem
    {
    public:
        struct AccessList : BaseAccessList
        {
            using Get = be::Components<MyComponent>;
        };

    public:
        TestSystem(bool& wasCalled)
            : m_wasCalled(wasCalled)
        {}

        void Run(const be::View<AccessList>&)
        {
            m_wasCalled = true;
        };

    private:
        bool& m_wasCalled;
    };

#ifndef TEST_SYSTEM
    #define TEST_SYSTEM(system_class_name)                \
        class system_class_name final : public TestSystem \
        {                                                 \
        public:                                           \
            system_class_name(bool& wasCalled)            \
                : TestSystem(wasCalled)                   \
            {}                                            \
        }
#endif

    TEST_SYSTEM(TestSystem1);
    TEST_SYSTEM(TestSystem2);
    TEST_SYSTEM(TestSystem3);

    TEST_F(SystemsSchedulerTest, UpdateWillRunOneSystemInOneGroup)
    {
        bool wasCalled = false;

        World world;
        SystemsScheduler sut(world);
        auto group = sut.CreateGroup();
        group.AttachSystem<TestSystem1>(std::ref(wasCalled));

        std::vector<SystemsScheduler::Group> groups{group};
        sut.Prepare(groups);
        sut.Update();

        ASSERT_TRUE(wasCalled);
    }

    TEST_F(SystemsSchedulerTest, UpdateWillRunAllSystemsInOneGroup)
    {
        bool wasCalled1 = false;
        bool wasCalled2 = false;
        bool wasCalled3 = false;

        World world;
        SystemsScheduler sut(world);
        auto group = sut.CreateGroup();
        group.AttachSystem<TestSystem1>(std::ref(wasCalled1));
        group.AttachSystem<TestSystem2>(std::ref(wasCalled2));
        group.AttachSystem<TestSystem3>(std::ref(wasCalled3));

        std::vector<SystemsScheduler::Group> groups{group};
        sut.Prepare(groups);
        sut.Update();

        ASSERT_TRUE(wasCalled1);
        ASSERT_TRUE(wasCalled2);
        ASSERT_TRUE(wasCalled3);
    }

    TEST_F(SystemsSchedulerTest, PrepareWillThrowIfAlreadyCalled)
    {
        World world;
        SystemsScheduler sut(world);
        auto group = sut.CreateGroup();
        group.AttachSystem<TestSystem1>(true);

        std::vector<SystemsScheduler::Group> groups{group};
        sut.Prepare(groups);

        ASSERT_THROW(sut.Prepare(groups), std::runtime_error);
    }

    TEST_F(SystemsSchedulerGroupTest, AttachSystemWillThrowIfSystemAlreadyAttachedToTheSameGroup)
    {
        World world;
        SystemsScheduler scheduler(world);

        auto sut = scheduler.CreateGroup();
        sut.AttachSystem<TestSystem1>(true);
        ASSERT_THROW(sut.AttachSystem<TestSystem1>(true), std::runtime_error);
    }

    TEST_F(SystemsSchedulerGroupTest, AttachSystemWillThrowIfSystemAlreadyAttachedToDifferentGroup)
    {
        World world;
        SystemsScheduler scheduler(world);

        auto group1 = scheduler.CreateGroup();
        group1.AttachSystem<TestSystem1>(true);

        auto group2 = scheduler.CreateGroup();
        ASSERT_THROW(group2.AttachSystem<TestSystem1>(true), std::runtime_error);
    }

    struct PlayerTag
    {
    };

    struct BulletTag
    {
    };

    class MultiViewTestSystem final
    {
    public:
        struct CreateTransformAL : be::BaseAccessList
        {
            using Add = be::Components<be::Transform, BulletTag>;
        };

        struct GetTransformAL : be::BaseAccessList
        {
            using Get = be::Components<be::Transform, PlayerTag>;
        };

        MultiViewTestSystem(bool& wasCalled)
            : m_wasCalled(wasCalled)
        {}

        void Run(const be::View<CreateTransformAL>& createView, const be::View<GetTransformAL>& getView)
        {
            for (const be::Entity entity : getView)
            {
                const auto& transform = getView.GetComponent<be::Transform>(entity);

                const auto bulletEntity = createView.CreateEntity(transform);
                createView.AddTag<BulletTag>(bulletEntity);
            }

            m_wasCalled = true;
        }

    private:
        bool& m_wasCalled;
    };

    TEST_F(SystemsSchedulerTest, WillProperlyScheduleSystemWithMultipleViews)
    {
        // Create player entity
        struct CreatePlayerAL : be::BaseAccessList
        {
            using Add = be::Components<be::Transform, PlayerTag>;
        };

        struct GetBulletAL : be::BaseAccessList
        {
            using Get = be::Components<be::Transform, BulletTag>;
        };

        bool wasCalled = false;
        be::Transform expectedTransform{.position = {123.0f, -1232.0f}};

        be::World world{};

        const auto playerView = world.CreateView<CreatePlayerAL>();
        const auto playerEntity = playerView.CreateEntity(expectedTransform);
        playerView.AddTag<PlayerTag>(playerEntity);

        be::SystemsScheduler scheduler{world};
        auto group = scheduler.CreateGroup();
        group.AttachSystem<MultiViewTestSystem>(std::ref(wasCalled));

        scheduler.Prepare({group});

        const auto bulletView = world.CreateView<GetBulletAL>();
        ASSERT_EQ(0, bulletView.EntitiesCount());

        scheduler.Update();
        ASSERT_EQ(1, bulletView.EntitiesCount());

        const be::Entity bulletEntity = *bulletView.begin();
        const auto& actualTransform = bulletView.GetComponent<be::Transform>(bulletEntity);

        ASSERT_EQ(expectedTransform.position, actualTransform.position);
        ASSERT_TRUE(wasCalled);
    }
} // namespace be::tests::unit
