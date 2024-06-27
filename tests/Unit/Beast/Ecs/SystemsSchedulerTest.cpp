#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/Components/Graphics.h>

#include <gtest/gtest.h>

#include <vector>
#include <typeinfo>
#include <typeindex>

#include <queue>

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

        enum class FunctionRelation
        {
            NONE,
            DEPENDENCY,
            DEPENDANT,
            WEAK_DEPENDENCY,
        };

        FunctionRelation GetFunctionRelation(const Component& component, const SystemFunction& other) const
        {
            const auto foundComponent = std::find(other.m_components.begin(), other.m_components.end(), component);
            if (foundComponent == other.m_components.end())
            {
                return FunctionRelation::NONE;
            }

            return EvaluateRelation(component.access, foundComponent->access);
        }

        void AddDependency(SystemFunction* function)
        {
            m_dependencies.push_back(function);
            function->m_dependants.push_back(this);
        }

        void AddWeakDependency(SystemFunction* function)
        {
            m_weakDependencies.push_back(function);
        }

        void AddComponent(Component&& component)
        {
            if (std::find(m_components.begin(), m_components.end(), component) == m_components.end())
            {
                m_components.emplace_back(std::move(component));
            }
        }

        void ResolveWeakDependencies()
        {
            if (m_weakDependencies.empty())
            {
                return;
            }

            // For each of the weak dependencies:
            // Check if it is in our graph.
            for (size_t i = 0; i < m_weakDependencies.size(); ++i)
            {
                auto* weakDependency = m_weakDependencies[i];
                const auto removeWeakDependency = [&]() {
                    m_weakDependencies[i] = m_weakDependencies.back();
                    m_weakDependencies.pop_back();
                };

                bool dependencyRemoved = false;

                std::unordered_set<SystemFunction*> visited{};
                std::queue<SystemFunction*> queue{};
                queue.push(this);
                while (!queue.empty())
                {
                    auto* function = queue.front();
                    queue.pop();
                    if (visited.contains(function))
                    {
                        continue;
                    }
                    visited.insert(function);

                    // If one of the functions in our dependencies chain has our weak dependency as an edge,
                    // remove our weak dependency as there's already an indirect edge via one of our direct edges.
                    if (function == weakDependency)
                    {
                        removeWeakDependency();
                        dependencyRemoved = true;
                        break;
                    }

                    for (auto* dependency : function->m_dependencies)
                    {
                        queue.push(dependency);
                    }

                    for (auto* dependant : function->m_dependants)
                    {
                        queue.push(dependant);
                    }
                }

                if (!dependencyRemoved)
                {
                    // Weak dependency not found in the graph. We need to decide manually
                    weakDependency->AddDependency(this);
                    removeWeakDependency();
                }
            }
        }

    private:
        static FunctionRelation EvaluateRelation(SystemFunction::ComponentAccess lhsAccess, SystemFunction::ComponentAccess rhsAccess)
        {
            using Access = SystemFunction::ComponentAccess;

            // For components with the same access (except GET), we make RHS a weak dependency of the LHS by default.
            if (lhsAccess == rhsAccess)
            {
                // Right Hand Side also GET the component, so we don't need to depend on each other.
                if (lhsAccess == Access::GET)
                {
                    return FunctionRelation::NONE;
                }

                // Other access, let's make a weak dependency as we can't decide which one should run prior to the other yet.
                return FunctionRelation::WEAK_DEPENDENCY;
            }

            // If RHS has it in any list except for GET, make it our dependency.
            if (lhsAccess == Access::GET)
            {
                // Right Hand Side modifes the component, hance, we depend on it.
                return FunctionRelation::DEPENDENCY;
            }

            // If RHS has it in REMOVE or ADD, make it a dependency.
            // Otherwise, make a dependant.
            if (lhsAccess == Access::UPDATE)
            {
                if (rhsAccess == Access::GET)
                {
                    // Right Hand Side reads the component we're modifying, it has to be our dependant.
                    return FunctionRelation::DEPENDANT;
                }

                // Right Hand Side adds or removes the component. We need to depend on it.
                return FunctionRelation::DEPENDENCY;
            }

            // If RHS has it in REMOVE, make it a dependency.
            // Otherwise, make a dependant.
            if (lhsAccess == Access::ADD)
            {
                if (rhsAccess == Access::REMOVE)
                {
                    // Right Hand Side removes the component we're adding. We need to depend on it.
                    return FunctionRelation::DEPENDENCY;
                }

                // Right Hand Side depends on us adding the component. It needs to depend on us.
                return FunctionRelation::DEPENDANT;
            }

            // For REMOVE, make RHS a dependant.
            // We have the component in REMOVE list at this point, so make RHS a dependant.
            return FunctionRelation::DEPENDANT;
        }

        bool HasEdge(const SystemFunction* searchedFunction) const
        {
            return std::find(m_dependencies.begin(), m_dependencies.end(), searchedFunction) != m_dependencies.end() || std::find(m_dependants.begin(), m_dependants.end(), searchedFunction) != m_dependencies.end();
        }

    public:
        std::string_view m_name;
        Wrapper m_implementation;

        std::vector<SystemFunction*> m_dependencies;
        std::vector<SystemFunction*> m_dependants;
        std::vector<SystemFunction*> m_weakDependencies;

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

            ResolveWeakDependencies();
            SetStarterFunctions();
        }

    private:
        void CheckDependencies(SystemFunction& lhs, SystemFunction& rhs)
        {
            using Relation = SystemFunction::FunctionRelation;
            Relation currentRelation = Relation::NONE;

            for (const auto& component : lhs.m_components)
            {
                const auto relation = lhs.GetFunctionRelation(component, rhs);

                // The relation is not changed, so we can ignore it.
                if (relation == Relation::NONE || relation == currentRelation)
                {
                    continue;
                }

                // We already have a stronger relation in place, so we can ignore the WEAK_DEPENDENCY
                if (relation == Relation::WEAK_DEPENDENCY && currentRelation != Relation::NONE)
                {
                    continue;
                }

                // Relation changed, we should update it.
                currentRelation = relation;
            }

            if (currentRelation == Relation::NONE)
            {
                return;
            }

            if (currentRelation == Relation::DEPENDENCY)
            {
                lhs.AddDependency(&rhs);
            }
            else if (currentRelation == Relation::DEPENDANT)
            {
                rhs.AddDependency(&lhs);
            }
            else
            {
                lhs.AddWeakDependency(&rhs);
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

        void ResolveWeakDependencies()
        {
            for (auto& function : m_functions)
            {
                function.ResolveWeakDependencies();
            }
        }

        void SetStarterFunctions()
        {
            for (size_t i = 0; i < m_starterFunctions.size(); ++i)
            {
                auto* starterFn = m_starterFunctions[i];
                if (!starterFn->m_dependencies.empty())
                {
                    m_starterFunctions[i] = m_starterFunctions.back();
                    m_starterFunctions.pop_back();
                }
            }

            if (m_starterFunctions.empty())
            {
                throw std::runtime_error("No started functions!");
            }
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
        struct Functions
        {
            struct Player
            {};

            struct Mover
            {};

            struct Bullet
            {};

            struct PlayerSpawnerAL : be::BaseAccessList
            {
                using Add = be::Components<Player, Mover, be::Transform, be::Sprite>;
            };
            static void PlayerSpawner(const be::View<PlayerSpawnerAL>&) {}

            struct PlayerMoverAL : be::BaseAccessList
            {
                using Update = be::Components<Mover, be::Transform>;
                using Get = be::Components<Player>;
            };
            static void PlayerMover(const be::View<PlayerMoverAL>&) {}

            struct GetPlayerDetailsAL : be::BaseAccessList
            {
                using Get = be::Components<Player, Mover, be::Transform>;
            };
            struct SpawnBulletAL : be::BaseAccessList
            {
                using Add = be::Components<Bullet, be::Transform, be::Sprite>;
            };
            static void PlayerShooter(const be::View<GetPlayerDetailsAL>&, const be::View<SpawnBulletAL>&) {}

            struct BulletMoverAL : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
                using Get = be::Components<Bullet>;
            };
            static void BulletMover(const be::View<BulletMoverAL>&) {}
        };

        // Also, we must make sure that the order of registeration always gives the same schedule!
        // Or at least, guarantee that functions with strong dependencies will always end up in the same place in the schedule no matter the order
        SystemsSchedulerWIP sut{};
        sut.RegisterFunction("PlayerSpawner", Functions::PlayerSpawner);
        sut.RegisterFunction("PlayerMover", Functions::PlayerMover);
        sut.RegisterFunction("PlayerShooter", Functions::PlayerShooter);
        sut.RegisterFunction("BulletMover", Functions::BulletMover);

        sut.Prepare();

        /**
         * In this test, there should be 1 starting function, the PlayerSpawner. All other functions should depend on it.
         * Essentially the schedule should look like this:
         *          |-> PlayerSpawner <-|
         *          |         |         | 
         *          |   PlayerShooter   |
         *          |   /           \   |
         *      BulletMover ------ PlayerMover
         */

        ASSERT_EQ(1, sut.m_starterFunctions.size());
        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ("PlayerSpawner", starterFn->m_name);
        ASSERT_TRUE(starterFn->m_dependencies.size() == 0);
        ASSERT_TRUE(starterFn->m_dependants.size() == 3);

        ASSERT_TRUE(HasDependant(starterFn, "PlayerMover"));
        ASSERT_TRUE(HasDependant(starterFn, "PlayerShooter"));
        ASSERT_TRUE(HasDependant(starterFn, "BulletMover"));

        const auto* playerMover = GetFunction(starterFn->m_dependants, "PlayerMover");
        ASSERT_TRUE(playerMover->m_dependencies.size() == 2);
        ASSERT_TRUE(HasDependency(playerMover, "PlayerSpawner"));
        ASSERT_TRUE(HasDependency(playerMover, "PlayerShooter"));

        const auto* playerShooter = GetFunction(starterFn->m_dependants, "PlayerShooter");

        ASSERT_TRUE(playerShooter->m_dependencies.size() == 1);
        ASSERT_TRUE(HasDependency(playerShooter, "PlayerSpawner"));

        const auto* bulletMover = GetFunction(starterFn->m_dependants, "BulletMover");

        ASSERT_TRUE(HasDependency(bulletMover, "PlayerSpawner"));
        ASSERT_TRUE(HasDependency(bulletMover, "PlayerShooter"));
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_WillResolveWeakDependencyViaIndirectStrongDependency)
    {
        struct Functions
        {
            struct Component
            {};

            struct FnAAccessList : be::BaseAccessList
            {
                using Add = be::Components<be::Transform, Component>;
            };
            static void FnA(const be::View<FnAAccessList>&){};

            struct FnBAccessList : be::BaseAccessList
            {
                using Add = be::Components<Component>;
                using Get = be::Components<be::Sprite>;
            };
            static void FnB(const be::View<FnBAccessList>&){};

            struct FnCAccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
            };
            static void FnC(const be::View<FnCAccessList>&){};

            struct FnDAccessList : be::BaseAccessList
            {
                using Get = be::Components<be::Transform>;
                using Add = be::Components<be::Sprite>;
            };
            static void FnD(const be::View<FnDAccessList>&){};
        };

        SystemsSchedulerWIP sut{};

        sut.RegisterFunction("FnA", Functions::FnA);
        sut.RegisterFunction("FnB", Functions::FnB);
        sut.RegisterFunction("FnC", Functions::FnC);
        sut.RegisterFunction("FnD", Functions::FnD);

        // Shuffle the functions to make sure the order of registration doesn't matter
        auto rd = std::random_device{};
        auto rng = std::default_random_engine{rd()};
        std::shuffle(sut.m_functions.begin(), sut.m_functions.end(), rng);

        for (size_t i = 0; i < sut.m_functions.size(); ++i)
        {
            const auto& fn = sut.m_functions[i];
            std::cout << "Function " << i << " " << fn.m_name << "\n";
        }

        sut.Prepare();
        ASSERT_EQ(1, sut.m_starterFunctions.size());

        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ("FnA", starterFn->m_name);

        ASSERT_EQ(0, starterFn->m_dependencies.size());
        ASSERT_EQ(2, starterFn->m_dependants.size());

        ASSERT_TRUE(HasDependant(starterFn, "FnC"));
        ASSERT_TRUE(HasDependant(starterFn, "FnD"));

        ASSERT_FALSE(HasDependant(starterFn, "FnB"));
        ASSERT_FALSE(HasDependency(starterFn, "FnB"));

        ASSERT_EQ(0, starterFn->m_weakDependencies.size());

        auto* fnC = GetFunction(starterFn->m_dependants, "FnC");
        ASSERT_TRUE(HasDependant(fnC, "FnD"));
        ASSERT_TRUE(fnC->m_dependants.size() == 1);
        ASSERT_TRUE(HasDependency(fnC, "FnA"));

        auto* fnD = GetFunction(fnC->m_dependants, "FnD");
        ASSERT_TRUE(HasDependant(fnD, "FnB"));
        ASSERT_EQ(1, fnD->m_dependants.size());
        ASSERT_TRUE(HasDependency(fnD, "FnA"));

        auto* fnB = GetFunction(fnD->m_dependants, "FnB");
        ASSERT_EQ(0, fnB->m_dependants.size());
        ASSERT_EQ(1, fnB->m_dependencies.size());
        ASSERT_TRUE(HasDependency(fnB, "FnD"));

        ASSERT_FALSE(HasDependency(fnB, "FnA"));
        ASSERT_FALSE(HasDependant(fnB, "FnA"));
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_WillIgnoreWeakDependencyIfItsNotInTheGraph)
    {
        struct Functions
        {
            struct Component
            {};

            struct FnAAccessList : be::BaseAccessList
            {
                using Add = be::Components<be::Transform, Component>;
            };
            static void FnA(const be::View<FnAAccessList>&){};

            struct FnBAccessList : be::BaseAccessList
            {
                using Add = be::Components<Component>;
            };
            static void FnB(const be::View<FnBAccessList>&){};

            struct FnCAccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
            };
            static void FnC(const be::View<FnCAccessList>&){};

            struct FnDAccessList : be::BaseAccessList
            {
                using Get = be::Components<be::Transform>;
                using Add = be::Components<be::Sprite>;
            };
            static void FnD(const be::View<FnDAccessList>&){};
        };

        SystemsSchedulerWIP sut{};

        sut.RegisterFunction("FnA", Functions::FnA);
        sut.RegisterFunction("FnB", Functions::FnB);
        sut.RegisterFunction("FnC", Functions::FnC);
        sut.RegisterFunction("FnD", Functions::FnD);

        // In this case, the order of registration actually matters as both functions have the same number of dependencies, so the first one in the list is going to be chosen.

        sut.Prepare();
        ASSERT_EQ(1, sut.m_starterFunctions.size());

        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ("FnA", starterFn->m_name);

        ASSERT_EQ(0, starterFn->m_dependencies.size());
        ASSERT_EQ(3, starterFn->m_dependants.size());

        ASSERT_TRUE(HasDependant(starterFn, "FnB"));
        ASSERT_TRUE(HasDependant(starterFn, "FnC"));
        ASSERT_TRUE(HasDependant(starterFn, "FnD"));

        ASSERT_FALSE(HasDependency(starterFn, "FnB"));

        ASSERT_EQ(0, starterFn->m_weakDependencies.size());

        auto* fnB = GetFunction(starterFn->m_dependants, "FnB");
        ASSERT_EQ(0, fnB->m_dependants.size());
        ASSERT_EQ(1, fnB->m_dependencies.size());
        ASSERT_TRUE(HasDependency(fnB, "FnA"));

        // TODO: Add test case to make sure that the function with less dependencies is chosen as the first one.
    }
} // namespace be::tests::unit
