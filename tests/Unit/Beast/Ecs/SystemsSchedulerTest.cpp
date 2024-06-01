#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>

#include <gtest/gtest.h>

#include <vector>
#include <typeinfo>
#include <typeindex>

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

    template<typename T>
    concept WorldView = requires {
        std::is_same<typename std::decay_t<T>::AL, be::BaseAccessList>::value;
    };

    using ComponentId = std::type_index;

    struct SystemFunction
    {
        enum class ComponentAccess
        {
            GET = 0,
            UPDATE = 1,
            ADD = 2,
            REMOVE = 3,
        };

        struct Component
        {
            ComponentId id;
            ComponentAccess access;

            bool operator==(const Component& rhs) const
            {
                return id == rhs.id;
            }
        };

        using Wrapper = std::function<void(be::World&)>;

        template<WorldView... Views>
        static SystemFunction Create(std::string_view name, Wrapper&& function)
        {
            SystemFunction systemFunction{name, std::move(function)};
            systemFunction.Process<Views...>();
            systemFunction.SortComponents();

            return systemFunction;
        }

        template<WorldView ViewT>
        void Process()
        {
            using ViewType = typename std::decay_t<ViewT>;

            using Get = ViewType::AL::Get;
            using Update = ViewType::AL::Update;
            using Add = ViewType::AL::Add;
            using Remove = ViewType::AL::Remove;

            // The order is important, because we don't want duplicates of the components
            // (which can happen if we have multiple views) and we also want to make sure that the components in case of duplicates,
            // end up in the most "restricting" access list.
            ProcessComponents(Remove{}, ComponentAccess::REMOVE);
            ProcessComponents(Add{}, ComponentAccess::ADD);
            ProcessComponents(Update{}, ComponentAccess::UPDATE);
            ProcessComponents(Get{}, ComponentAccess::GET);
        }

        template<typename... Components>
        void ProcessComponents(be::Components<Components...> types, ComponentAccess access)
        {
            if constexpr (types.size != 0)
            {
                AddComponent({std::type_index(typeid(Components))..., access});
            }
        }

        void SortComponents()
        {
            // We sort the components so that we have components from least restricing to most restricing (GET -> REMOVE).
            // This way we can quickly stop processing the functions dependencies.

            std::sort(m_components.begin(), m_components.end(), [](const Component& lhs, const Component& rhs) {
                return lhs.access < rhs.access;
            });
        }

        void AddDependency(SystemFunction* function)
        {
            m_dependencies.push_back(function);
            function->m_dependants.push_back(this);
        }

        void AddComponent(Component&& component)
        {
            if (std::find(m_components.begin(), m_components.end(), component) == m_components.end())
            {
                m_components.emplace_back(std::move(component));
            }
        }

    public:
        std::string_view m_name;
        Wrapper m_function;

        std::vector<SystemFunction*> m_dependencies;
        std::vector<SystemFunction*> m_dependants;

        std::vector<Component> m_components;
    };

    class SystemsSchedulerWIP
    {
    public:
        template<typename... Views>
        using NonMemberFn = void (*)(Views...);

        template<typename System, typename... Views>
        using MemberFn = void (System::*)(Views...);

        template<typename... Views>
        void RegisterFunction(std::string_view name, NonMemberFn<Views...> systemFunction)
        {
            auto wrapperFunction = [function = std::move(systemFunction)](be::World& world) {
                function(world.CreateView<typename std::decay_t<Views>::AL>()...);
            };
            m_functions.emplace_back(SystemFunction::Create<Views...>(name, std::move(wrapperFunction)));
        }

        /*template<typename System, typename... Views>
        void RegisterFunction(std::string_view name, MemberFn<System, Views...> systemFunction)
        {
            m_functions.emplace_back(SystemFunction<Views...>(name, systemFunction));
        }*/

        void Prepare()
        {
            const auto functionsCount = m_functions.size();
            for (std::size_t i = 0; i < functionsCount; ++i)
            {
                auto& lhsFunction = m_functions[i];
                for (std::size_t j = i + 1; j < functionsCount; ++j)
                {
                    CheckDependencies(lhsFunction, m_functions[j]);
                }

                if (lhsFunction.m_dependencies.empty())
                {
                    m_starterFunctions.push_back(&lhsFunction);
                }
            }
        }

    private:
        void CheckDependencies(SystemFunction& lhs, SystemFunction& rhs)
        {
            for (const auto& component : lhs.m_components)
            {
                const auto foundComponent = std::find(rhs.m_components.begin(), rhs.m_components.end(), component);
                if (foundComponent == rhs.m_components.end())
                {
                    // No dependencies on this component, let's keep comparing.
                    continue;
                }

                if (HaveDependencies(&lhs, component.access, &rhs, foundComponent->access))
                {
                    // Dependencies found, no need to check other components.
                    break;
                }
            }
        }

        bool HaveDependencies(SystemFunction* lhs, SystemFunction::ComponentAccess lhsAccess, SystemFunction* rhs, SystemFunction::ComponentAccess rhsAccess)
        {
            using Access = SystemFunction::ComponentAccess;

            // If RHS has it in any list except for GET, make it a dependency.
            if (lhsAccess == Access::GET)
            {
                if (rhsAccess != Access::GET)
                {
                    // Right Hand Side modifes the component, hance, we depend on it, no need to check other components.
                    lhs->AddDependency(rhs);
                    return true;
                }

                // Right Hand Side also GET the component, so we don't need to depend on each other.
                return false;
            }

            // If RHS has it in REMOVE or ADD, make it a dependency.
            // Otherwise, make a dependant.
            if (lhsAccess == Access::UPDATE)
            {
                if (rhsAccess == Access::GET)
                {
                    // Right Hand Side reads the component we're modifying, it has to be our dependant.
                    rhs->AddDependency(lhs);
                    return true;
                }

                if (rhsAccess == Access::UPDATE)
                {
                    // Right Hand Side also updates the component we're updating. Let's make RHS a dependant.
                    // For components with the same access (except GET), we make RHS dependant of the LHS by default.
                    rhs->AddDependency(lhs);
                    return true;
                }

                // Right Hand Side adds or removes the component. We need to depend on it.
                lhs->AddDependency(rhs);
                return true;
            }

            // If RHS has it in REMOVE, make it a dependency.
            // Otherwise, make a dependant.
            if (lhsAccess == Access::ADD)
            {
                if (rhsAccess == Access::REMOVE)
                {
                    // Right Hand Side removes the component we're adding. We need to depend on it.
                    lhs->AddDependency(rhs);
                }
                else
                {
                    // Right Hand Side depends on us adding the component. It needs to depend on us.
                    rhs->AddDependency(lhs);
                }
                return true;
            }

            // For REMOVE, make RHS a dependant.
            // For components with the same access (except GET), we make RHS dependant of the LHS by default.

            // We have the component in REMOVE list at this point, so make RHS a dependant.
            lhs->AddDependency(rhs);
            return true;
        }

    public:
        std::vector<SystemFunction> m_functions;
        std::vector<SystemFunction*> m_starterFunctions;
    };

#define BE_SINGLE_COMPONENT_AL(Type)                \
    struct AL##Type : be::BaseAccessList            \
    {                                               \
        using Type = be::Components<be::Transform>; \
    };

    BE_SINGLE_COMPONENT_AL(Get);
    static void getFunction(const be::View<ALGet>&) {}

    BE_SINGLE_COMPONENT_AL(Update);
    static void updateFunction(const be::View<ALUpdate>&) {}

    BE_SINGLE_COMPONENT_AL(Add);
    static void addFunction(const be::View<ALAdd>&) {}

    BE_SINGLE_COMPONENT_AL(Remove);
    static void removeFunction(const be::View<ALRemove>&) {}

    TEST_F(SystemsSchedulerTest, AutomaticScheduling)
    {
        SystemsSchedulerWIP scheduler{};
        scheduler.RegisterFunction("getFunction", getFunction);
        scheduler.RegisterFunction("updateFunction", updateFunction);
        scheduler.RegisterFunction("addFunction", addFunction);
        scheduler.RegisterFunction("removeFunction", removeFunction);

        scheduler.Prepare();

        ASSERT_TRUE(scheduler.m_starterFunctions.size() == 1);
        auto* starterFn = scheduler.m_starterFunctions[0];
        ASSERT_EQ("removeFunction", starterFn->m_name);
        ASSERT_TRUE(starterFn->m_dependencies.size() == 0);
        ASSERT_TRUE(starterFn->m_dependants.size() == 3);

        /*scheduler.Prepare();
        scheduler.Run();
        scheduler.Update();*/
    }
} // namespace be::tests::unit
