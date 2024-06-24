#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/Components/Graphics.h>

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
            std::string name;

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

        template<WorldView... Views>
        void Process()
        {
            using Get = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Get...>>;
            using Update = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Update...>>;
            using Add = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Add...>>;
            using Remove = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Remove...>>;

            // The order is important, because we don't want duplicates of the components
            // (which can happen if we have multiple views) and we also want to make sure that the components - in case of duplicates -
            // end up in the most "restricting" access list.
            ProcessComponents(Remove{}, ComponentAccess::REMOVE);
            ProcessComponents(Add{}, ComponentAccess::ADD);
            ProcessComponents(Update{}, ComponentAccess::UPDATE);
            ProcessComponents(Get{}, ComponentAccess::GET);
        }

        template<typename... ComponentsT>
        void ProcessComponents(be::Components<ComponentsT...> types, ComponentAccess access)
        {
            if constexpr (types.size != 0)
            {
                (AddComponent({std::type_index(typeid(ComponentsT)), access, typeid(ComponentsT).name()}), ...);
            }
        }

        void SortComponents()
        {
            // We sort the components so that we have components from least restricing to most restricing (GET -> REMOVE).
            // This way we can quickly stop processing the functions dependencies when creating dependencies graph.
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
        Wrapper m_implementation;

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

            if (m_starterFunctions.empty())
            {
                throw std::runtime_error("No started functions!");
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

            // If RHS has it in any list except for GET, make it our dependency.
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

    static const SystemFunction* GetFunction(std::span<const SystemFunction* const> source, std::string_view functionToFind)
    {
        const auto foundIt = std::find_if(source.begin(), source.end(), [functionToFind](const SystemFunction* dependency) {
            return dependency->m_name == functionToFind;
        });

        return foundIt == source.end() ? nullptr : *foundIt;
    }

    static bool HasDependant(const SystemFunction* function, std::string_view expectedDependant)
    {
        return GetFunction(function->m_dependants, expectedDependant) != nullptr;
    }

    static bool HasDependency(const SystemFunction* function, std::string_view expectedDependency)
    {
        return GetFunction(function->m_dependencies, expectedDependency) != nullptr;
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling)
    {
        struct Functions
        {
            BE_SINGLE_COMPONENT_AL(Get);
            static void getFunction(const be::View<ALGet>&) {}

            BE_SINGLE_COMPONENT_AL(Update);
            static void updateFunction(const be::View<ALUpdate>&) {}

            BE_SINGLE_COMPONENT_AL(Add);
            static void addFunction(const be::View<ALAdd>&) {}

            BE_SINGLE_COMPONENT_AL(Remove);
            static void removeFunction(const be::View<ALRemove>&) {}
        };

        SystemsSchedulerWIP scheduler{};
        scheduler.RegisterFunction("getFunction", Functions::getFunction);
        scheduler.RegisterFunction("updateFunction", Functions::updateFunction);
        scheduler.RegisterFunction("addFunction", Functions::addFunction);
        scheduler.RegisterFunction("removeFunction", Functions::removeFunction);

        scheduler.Prepare();

        ASSERT_TRUE(scheduler.m_starterFunctions.size() == 1);
        auto* starterFn = scheduler.m_starterFunctions[0];
        ASSERT_EQ("removeFunction", starterFn->m_name);
        ASSERT_TRUE(starterFn->m_dependencies.size() == 0);
        ASSERT_TRUE(starterFn->m_dependants.size() == 3);

        ASSERT_TRUE(HasDependant(starterFn, "updateFunction"));
        ASSERT_TRUE(HasDependant(starterFn, "addFunction"));
        ASSERT_TRUE(HasDependant(starterFn, "getFunction"));

        const auto* addFn = GetFunction(starterFn->m_dependants, "addFunction");
        ASSERT_TRUE(HasDependency(addFn, "removeFunction"));
        ASSERT_TRUE(HasDependant(addFn, "updateFunction"));
        ASSERT_TRUE(HasDependant(addFn, "getFunction"));

        const auto* updateFn = GetFunction(addFn->m_dependants, "updateFunction");

        ASSERT_TRUE(HasDependency(updateFn, "addFunction"));
        ASSERT_TRUE(HasDependency(updateFn, "removeFunction"));
        ASSERT_TRUE(HasDependant(updateFn, "getFunction"));

        const auto* getFn = GetFunction(updateFn->m_dependants, "getFunction");
        ASSERT_TRUE(getFn->m_dependants.empty());
        ASSERT_TRUE(HasDependency(getFn, "removeFunction"));
        ASSERT_TRUE(HasDependency(getFn, "addFunction"));
        ASSERT_TRUE(HasDependency(getFn, "updateFunction"));
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_WillReturnErrorWhenCannotSortFunctions)
    {
        /**
         * I want to test if preparing the schedule succeeds. The scheduler::Prepare() function should return `optional` or `expected` with schedule that can be run.
         * If there are circular dependencies inside the schedule, it should fail. It could also just throw an exception on failure, as this seems like a fatal error, that prevents the program from running.
         * Ok, so an exception is the way to go for that.
         * 
         * Now, the question is how to detect a circular dependency and how to prepare the test case for that.
         * It should be quite simple.
         * A circular dependency can happen when function A depends on a result of function B which depends on a result of function A. This is the simplest scenario.
         * A more complicated scenario includes a non-direct circular dependency. For example, function A depends on function B which depends on function C which depends on function A.
         * 
         * Let's try this out.
         */

        struct Component1
        {
        };

        struct Functions
        {
            struct AAL : be::BaseAccessList
            {
                using Update = be::Components<Component1>;
                using Get = be::Components<be::Transform>;
            };
            static void FunctionA(const be::View<AAL>&) {}

            struct BAL : be::BaseAccessList
            {
                using Get = be::Components<Component1>;
                using Update = be::Components<be::Transform>;
            };
            static void FunctionB(const be::View<BAL>&) {}
        };

        SystemsSchedulerWIP sut{};

        sut.RegisterFunction("functionA", Functions::FunctionA);
        sut.RegisterFunction("functionB", Functions::FunctionB);

        ASSERT_THROW(sut.Prepare(), std::runtime_error);
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_RealWorldScenario)
    {
        struct Player
        {
        };

        struct Mover
        {
        };

        struct Bullet
        {
        };

        class PlayerSpawner final
        {
        public:
            struct AccessList : be::BaseAccessList
            {
                using Add = be::Components<Player, Mover, be::Transform, be::Sprite>;
            };

            static void Run(const be::View<AccessList>&) {}
        };

        class PlayerMover final
        {
        public:
            struct AccessList : be::BaseAccessList
            {
                using Update = be::Components<Mover, be::Transform>;
                using Get = be::Components<Player>;
            };

            static void Run(const be::View<AccessList>&) {}
        };

        class PlayerShooter final
        {
        public:
            struct GetPlayerDetailsAL : be::BaseAccessList
            {
                using Get = be::Components<Player, Mover, be::Transform>;
            };

            struct SpawnBulletAL : be::BaseAccessList
            {
                using Add = be::Components<Bullet, be::Transform, be::Sprite>;
            };
            static void Run(const be::View<GetPlayerDetailsAL>&, const be::View<SpawnBulletAL>&) {}
        };

        class BulletMover final
        {
        public:
            struct AccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
                using Get = be::Components<Bullet>;
            };

            static void Run(const be::View<AccessList>&) {}
        };

        // TODO: Debug this
        // I believe the order should be:
        // PlayerSpawner -> PlayerMover/PlayerShooter -> BulletMover

        SystemsSchedulerWIP sut{};
        sut.RegisterFunction("PlayerSpawner", PlayerSpawner::Run);
        sut.RegisterFunction("PlayerMover", PlayerMover::Run);
        sut.RegisterFunction("PlayerShooter", PlayerShooter::Run);
        sut.RegisterFunction("BulletMover", BulletMover::Run);

        sut.Prepare();
    }
} // namespace be::tests::unit
