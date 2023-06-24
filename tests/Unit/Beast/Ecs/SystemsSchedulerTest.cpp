#include <Unit/Beast/Ecs/SystemsSchedulerTest.h>

#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>

namespace be::tests::unit
{
    struct MyComponent
    {
        int value = 0;
    };

    class MySystem1 final : public ISystem
    {
    public:
        struct AccessList : BaseAccessList
        {
            using Get = be::Components<MyComponent>;
        };

    public:
        MySystem1(bool& wasCalled)
            : m_wasCalled(wasCalled)
        {}

        void Run(const be::View<AccessList>&)
        {
            m_wasCalled = true;
        };

    private:
        bool& m_wasCalled;
    };

    TEST_F(SystemsSchedulerTest, UpdateWillRunOneSystemInOneGroup)
    {
        bool wasCalled = false;

        World world;
        SystemsScheduler sut(world);
        auto group = sut.CreateGroup();
        group.AttachSystem<MySystem1>(wasCalled);

        std::vector<SystemsScheduler::Group> groups{group};
        sut.Prepare(groups);
        sut.Update();

        ASSERT_TRUE(wasCalled);
    }
} // namespace be::tests::unit