#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/Components/Graphics.h>

#include <gtest/gtest.h>
#include <fmt/format.h>

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
        static SystemFunction Create(Id id, std::string_view name, Wrapper&& function)
        {
            SystemFunction systemFunction{id, name, std::move(function)};
            systemFunction.Process<Views...>();

            return systemFunction;
        }

        template<WorldView... Views>
        void Process()
        {
            using Get = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Get...>>;
            using Update = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Update...>>;
            using Add = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Add...>>;
            using Remove = typename entt::type_list_unique_t<entt::type_list_cat_t<typename std::decay_t<Views>::AL::Remove...>>;

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

        enum class FunctionRelation
        {
            NONE,
            PARENT,
            CHILD,
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

        void AddParent(SystemFunction* parentFunction)
        {
            m_parents.push_back(parentFunction);
            parentFunction->m_children.push_back(this);
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
            else
            {
                throw std::runtime_error("Components not unique");
            }
        }

        void RemoveParent(SystemFunction* parent)
        {
            auto& children = parent->m_children;
            for (std::size_t i = 0; i < children.size(); ++i)
            {
                if (children[i] == this)
                {
                    children[i] = children.back();
                    children.pop_back();

                    break;
                }
            }

            for (std::size_t i = 0; i < m_parents.size(); ++i)
            {
                if (m_parents[i] == parent)
                {
                    m_parents[i] = m_parents.back();
                    m_parents.pop_back();

                    break;
                }
            }
        }

        /**
         * @brief A BFS implementation that searches for a weak dependency in the graph of this function.
         * When dependency is found, it is removed from the list of weak dependencies, as this means there's already an indirect
         * connection between this function and that dependency.
         * 
         * Otherwise, there has to be a strong dependency between them, as they cannot run at the same time.
         * In that case, we simply convert the weak dependency into a strong dependency.
         */
        void ResolveWeakDependencies()
        {
            if (m_weakDependencies.empty())
            {
                return;
            }

            for (size_t i = 0; i < m_weakDependencies.size(); ++i)
            {
                const auto removeWeakDependency = [this](size_t i) {
                    m_weakDependencies[i] = m_weakDependencies.back();
                    m_weakDependencies.pop_back();
                };

                auto* weakDependency = m_weakDependencies[i];
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

                    // If one of the functions in the graph we're in has our weak dependency as an edge,
                    // remove our weak dependency as there's already an indirect edge via one of our direct edges.
                    if (function == weakDependency)
                    {
                        removeWeakDependency(i);
                        dependencyRemoved = true;
                        break;
                    }

                    for (auto* parent : function->m_parents)
                    {
                        queue.push(parent);
                    }

                    for (auto* child : function->m_children)
                    {
                        queue.push(child);
                    }
                }

                // Weak dependency not found in the graph. We need to decide manually.
                if (!dependencyRemoved)
                {
                    AddParent(weakDependency);
                    removeWeakDependency(i);
                }
            }
        }

        void SimplifyDependencies()
        {
            std::vector<SystemFunction*> queue = m_parents;
            while (!queue.empty())
            {
                std::unordered_set<const SystemFunction*> visitedNodes{};

                SystemFunction* searchedParent = queue.back();
                queue.pop_back();

                RemoveParent(searchedParent);
                if (!SearchParentDFS(searchedParent, m_parents, visitedNodes))
                {
                    AddParent(searchedParent);
                }
            }
        }

        bool SearchParentDFS(const SystemFunction* nodeToFind, std::span<SystemFunction* const> nodesToSearch, std::unordered_set<const SystemFunction*>& visitedNodes)
        {
            for (const auto* processedNode : nodesToSearch)
            {
                if (nodeToFind == processedNode)
                {
                    return true;
                }

                if (!visitedNodes.contains(processedNode))
                {
                    visitedNodes.insert(processedNode);
                    if (SearchParentDFS(nodeToFind, processedNode->m_parents, visitedNodes))
                    {
                        return true;
                    }
                }
            }

            return false;
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

            // At this point, we know for sure that lhsAccess and rhsAccess are NOT the same.

            // If we have it in GET, we need to depend on the RHS function,
            // because it means it has it in a higher access list, thus it modifies the component
            if (lhsAccess == Access::GET)
            {
                // Right Hand Side modifes the component, hance, we depend on it.
                return FunctionRelation::PARENT;
            }

            // If RHS has it in REMOVE or ADD, make it our parent.
            // Otherwise, make it our child.
            if (lhsAccess == Access::UPDATE)
            {
                if (rhsAccess == Access::GET)
                {
                    // Right Hand Side reads the component we're modifying, it has to be our child.
                    return FunctionRelation::CHILD;
                }

                // Right Hand Side adds or removes the component. We need to depend on it.
                return FunctionRelation::PARENT;
            }

            // If RHS has it in REMOVE, make it a parent.
            // Otherwise, make it a child.
            if (lhsAccess == Access::ADD)
            {
                if (rhsAccess == Access::REMOVE)
                {
                    // Right Hand Side removes the component we're adding. We need to depend on it.
                    return FunctionRelation::PARENT;
                }

                // Right Hand Side depends on us adding the component. It needs to depend on us.
                return FunctionRelation::CHILD;
            }

            // We have the component in REMOVE list at this point, so make RHS a child.
            return FunctionRelation::CHILD;
        }

    public:
        Id m_id;
        std::string_view m_name;
        Wrapper m_implementation;

        std::vector<SystemFunction*> m_parents;
        std::vector<SystemFunction*> m_children;
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
            m_functions.emplace_back(SystemFunction::Create<Views...>(Id{m_functions.size()}, name, std::move(wrapperFunction)));
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

                if (lhsFunction.m_parents.empty())
                {
                    m_starterFunctions.push_back(&lhsFunction);
                }
            }

            ResolveDependencies();
            SetUpStarterFunctions();
            VerifyGraphIsDAG();
        }

    private:
        void CheckDependencies(SystemFunction& lhs, SystemFunction& rhs)
        {
            using Relation = SystemFunction::FunctionRelation;
            Relation currentRelation = Relation::NONE;

            for (const auto& component : lhs.m_components)
            {
                const auto newRelation = lhs.GetFunctionRelation(component, rhs);

                // The relation is not changed, so we can ignore it.
                if (newRelation == Relation::NONE || newRelation == currentRelation)
                {
                    continue;
                }

                if (currentRelation != Relation::NONE)
                {
                    // We already have a stronger relation in place,
                    // so we can ignore the WEAK_DEPENDENCY.
                    if (newRelation == Relation::WEAK_DEPENDENCY)
                    {
                        continue;
                    }

                    // There's a strong dependency in both direction, which prevents us from creating a graph.
                    if (currentRelation != Relation::WEAK_DEPENDENCY)
                    {
                        // TODO: Make this error clear.
                        throw std::runtime_error("Functions depend on each other");
                    }
                }

                // Relation changed, we should update it.
                currentRelation = newRelation;
            }

            if (currentRelation == Relation::NONE)
            {
                return;
            }

            if (currentRelation == Relation::PARENT)
            {
                lhs.AddParent(&rhs);
            }
            else if (currentRelation == Relation::CHILD)
            {
                rhs.AddParent(&lhs);
            }
            else
            {
                lhs.AddWeakDependency(&rhs);
            }
        }

        void ResolveDependencies()
        {
            for (auto& function : m_functions)
            {
                function.ResolveWeakDependencies();
            }

            for (auto& function : m_functions)
            {
                function.SimplifyDependencies();
            }
        }

        void SetUpStarterFunctions()
        {
            std::size_t i = 0;
            while (i != m_starterFunctions.size())
            {
                auto* starterFn = m_starterFunctions[i];
                if (starterFn->m_parents.empty())
                {
                    ++i;
                    continue;
                }

                m_starterFunctions[i] = m_starterFunctions.back();
                m_starterFunctions.pop_back();
            }

            if (m_starterFunctions.empty())
            {
                throw std::runtime_error("No started functions!");
            }
        }

        void VerifyGraphIsDAG() const
        {
            // TODO: Rewrite this. This is crap :/

            using FnIndex = std::size_t;

            const auto graphSize = m_functions.size();
            std::vector<FnIndex> graph(graphSize, 0u);
            std::vector<FnIndex> startNodes;

            for (FnIndex i = 0; i < graphSize; ++i)
            {
                const auto parentsCount = m_functions[i].m_parents.size();
                graph[i] = parentsCount;

                if (parentsCount == 0)
                {
                    startNodes.push_back(i);
                }
            }
         
            std::vector<FnIndex> sortedGraph;
            sortedGraph.reserve(graphSize);

            while (!startNodes.empty())
            {
                const auto startNodeIndex = startNodes.back();
                sortedGraph.push_back(startNodeIndex);
                startNodes.pop_back();

                for (const auto& function : m_functions[startNodeIndex].m_children)
                {
                    const FnIndex functionIndex = function->m_id.Raw();
                    --graph[functionIndex];
                    
                    if (graph[functionIndex] == 0)
                    {
                        startNodes.push_back(functionIndex);
                        graph.erase(std::next(graph.begin(), functionIndex));
                    }
                }
            }

            if (graph.size())
            {
                throw std::runtime_error("NOT A DAG");
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
        const auto foundIt = std::find_if(source.begin(), source.end(), [functionToFind](const SystemFunction* function) {
            return function->m_name == functionToFind;
        });

        return foundIt == source.end() ? nullptr : *foundIt;
    }

    static bool HasChild(const SystemFunction* function, std::string_view expectedChild)
    {
        return GetFunction(function->m_children, expectedChild) != nullptr;
    }

    static bool HasParent(const SystemFunction* function, std::string_view expectedParent)
    {
        return GetFunction(function->m_parents, expectedParent) != nullptr;
    }

    static void ShuffleFunctions(SystemsSchedulerWIP& scheduler)
    {
        auto& functions = scheduler.m_functions;

        // Shuffle the functions to make sure the order of registration doesn't matter
        std::random_device rd{};
        std::default_random_engine rng{rd()};
        std::shuffle(functions.begin(), functions.end(), rng);

        for (size_t i = 0; i < functions.size(); ++i)
        {
            const auto& fn = functions[i];
            std::cout << fmt::format("Function {} {}\n", i, fn.m_name);
        }
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

        SystemsSchedulerWIP sut{};
        sut.RegisterFunction("getFunction", Functions::getFunction);
        sut.RegisterFunction("updateFunction", Functions::updateFunction);
        sut.RegisterFunction("addFunction", Functions::addFunction);
        sut.RegisterFunction("removeFunction", Functions::removeFunction);
        ShuffleFunctions(sut);

        sut.Prepare();

        ASSERT_EQ(1, sut.m_starterFunctions.size());
        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ("removeFunction", starterFn->m_name);
        ASSERT_EQ(0, starterFn->m_parents.size());
        ASSERT_EQ(1, starterFn->m_children.size());

        ASSERT_TRUE(HasChild(starterFn, "addFunction"));

        const auto* addFn = GetFunction(starterFn->m_children, "addFunction");
        ASSERT_EQ(1, addFn->m_parents.size());
        ASSERT_EQ(1, addFn->m_children.size());

        ASSERT_TRUE(HasParent(addFn, "removeFunction"));
        ASSERT_TRUE(HasChild(addFn, "updateFunction"));

        const auto* updateFn = GetFunction(addFn->m_children, "updateFunction");
        ASSERT_EQ(1, updateFn->m_parents.size());
        ASSERT_EQ(1, updateFn->m_children.size());

        ASSERT_TRUE(HasParent(updateFn, "addFunction"));
        ASSERT_TRUE(HasChild(updateFn, "getFunction"));

        const auto* getFn = GetFunction(updateFn->m_children, "getFunction");
        ASSERT_EQ(1, updateFn->m_parents.size());
        ASSERT_TRUE(getFn->m_children.empty());

        ASSERT_TRUE(HasParent(getFn, "updateFunction"));
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_WillThrowWhenTwoFunctionsStronglyDependOnEachOther)
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

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_WillThrowWhenThereIsCircularDependencyInGraph)
    {
        /**
         * In this scenario there's a circular dependency
         * FnA ---Component1---> FnC ---Component3---> FnB ---Component2---> FnA
         * 
         * There's starter function as well to make sure we get the exact error we need
         */

        struct Component1
        {};
        struct Component2
        {};
        struct Component3
        {};

        struct Functions
        {
            struct StarterAL : be::BaseAccessList
            {
                using Add = be::Components<be::Transform>;
            };
            static void StarterFn(const be::View<StarterAL>&) {}

            struct AAL : be::BaseAccessList
            {
                using Add = be::Components<Component1>;
                using Get = be::Components<Component2, be::Transform>;
            };
            static void FnA(const be::View<AAL>&) {}

            struct BAL : be::BaseAccessList
            {
                using Add = be::Components<Component2>;
                using Get = be::Components<Component3>;
            };
            static void FnB(const be::View<BAL>&) {}

            struct CAL : be::BaseAccessList
            {
                using Add = be::Components<Component3>;
                using Get = be::Components<Component1>;
            };
            static void FnC(const be::View<CAL>&) {}
        };

        SystemsSchedulerWIP sut{};

        sut.RegisterFunction("StarterFn", Functions::StarterFn);
        sut.RegisterFunction("FnA", Functions::FnA);
        sut.RegisterFunction("FnB", Functions::FnB);
        sut.RegisterFunction("FnC", Functions::FnC);
        ShuffleFunctions(sut);

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

            /**
             * 
             * Another important question is if it should actually be allowed to have the same component in two different access lists?
             * Perhaps this is a sign of a bad code?
             * The mover here could technically iterate over entities from the Get view and create new entities using the Spawn view, which could - in theory - modify the Get view.
             * I think to make sure this doesn't happen, we should prohibit that. Essentially, we want combine all Access Lists and make sure there aren't any duplicates.
             * This would happen on the compilation of `RegisterFunction` call I believe.
             * 
             * I think we should also allow adding/removing the component when it's in the Update list. This way, we can make those access lists much easier, because we can combine
             * the GetPlayerDetailsAL and SpawnBulletAL into a single AL like this:
             * struct SpawnBulletsAL
             * {
             *  using Add = be::Components<Bullet, be::Sprite>;
             *  using Update = be::Components<be::Transform>;
             *  using Get = be::Components<Player, Mover>; // This should also be replaced with Required in the future
             * };
             * 
             * And now, the dependencies are clear. It depends on the spawner via Player, Mover and Transform.
             * And it also depends strongly on the PlayerMover via Mover component.
             * This is a clean dependency chain, and lets us avoid two functions strongly referancing each-other. So this can potentially also be a fix to our problem where two functions strongly depend on each other on the same level.
             * This also seems to make more sense, as we not necessarily need to restrict the operations so much. We really care about writes not happening simutainously, but we can use Add and Remove to just add stronger ordering guarantess.
             * Phew... There's a lot(!!!) to think about with all this.
             */

            struct PlayerMoverAL : be::BaseAccessList
            {
                using Update = be::Components<Mover, be::Transform>;
                using Get = be::Components<Player>;
            };
            static void PlayerMover(const be::View<PlayerMoverAL>&) {}

            struct SpawnBulletsAL : be::BaseAccessList
            {
                using Add = be::Components<Bullet, be::Sprite>;
                using Update = be::Components<be::Transform>;
                using Get = be::Components<Player, Mover>;
            };
            static void PlayerShooter(const be::View<SpawnBulletsAL>&) {}

            struct BulletMoverAL : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
                using Get = be::Components<Bullet>;
            };
            static void BulletMover(const be::View<BulletMoverAL>&) {}
        };

        SystemsSchedulerWIP sut{};

        sut.RegisterFunction("PlayerSpawner", Functions::PlayerSpawner);
        sut.RegisterFunction("PlayerMover", Functions::PlayerMover);
        sut.RegisterFunction("BulletMover", Functions::BulletMover);
        sut.RegisterFunction("PlayerShooter", Functions::PlayerShooter);
        ShuffleFunctions(sut);

        sut.Prepare();

        /**
         * In this test, there should be 1 starting function, the PlayerSpawner. All other functions should depend on it.
         * Essentially the schedule should look like this:
         *          |-> PlayerSpawner <-|
         *          |         |         | 
         *          |   PlayerMover     |
         *          |         |         |
         *          |   PlayerShooter   |
         *          |         |         |
         *          |    BulletMover    |
         */

        ASSERT_EQ(1, sut.m_starterFunctions.size());
        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ("PlayerSpawner", starterFn->m_name);
        ASSERT_EQ(0, starterFn->m_parents.size());
        ASSERT_EQ(1, starterFn->m_children.size());

        ASSERT_TRUE(HasChild(starterFn, "PlayerMover"));

        const auto* playerMover = GetFunction(starterFn->m_children, "PlayerMover");
        ASSERT_EQ(1, playerMover->m_children.size());
        ASSERT_EQ(1, playerMover->m_parents.size());

        ASSERT_TRUE(HasParent(playerMover, "PlayerSpawner"));
        ASSERT_TRUE(HasChild(playerMover, "PlayerShooter"));

        const auto* playerShooter = GetFunction(playerMover->m_children, "PlayerShooter");

        ASSERT_EQ(1, playerShooter->m_children.size());
        ASSERT_EQ(1, playerShooter->m_parents.size());

        ASSERT_TRUE(HasParent(playerShooter, "PlayerMover"));

        ASSERT_TRUE(HasChild(playerShooter, "BulletMover"));

        const auto* bulletMover = GetFunction(playerShooter->m_children, "BulletMover");

        ASSERT_EQ(1, bulletMover->m_parents.size());
        ASSERT_EQ(0, bulletMover->m_children.size());

        ASSERT_TRUE(HasParent(bulletMover, "PlayerShooter"));
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

        sut.RegisterFunction("FnD", Functions::FnD);
        sut.RegisterFunction("FnA", Functions::FnA);
        sut.RegisterFunction("FnC", Functions::FnC);
        sut.RegisterFunction("FnB", Functions::FnB);
        ShuffleFunctions(sut);

        sut.Prepare();
        ASSERT_EQ(1, sut.m_starterFunctions.size());

        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ("FnA", starterFn->m_name);

        ASSERT_EQ(0, starterFn->m_parents.size());
        ASSERT_EQ(1, starterFn->m_children.size());

        ASSERT_TRUE(HasChild(starterFn, "FnC"));

        ASSERT_FALSE(HasChild(starterFn, "FnB"));
        ASSERT_FALSE(HasParent(starterFn, "FnB"));

        ASSERT_EQ(0, starterFn->m_weakDependencies.size());

        auto* fnC = GetFunction(starterFn->m_children, "FnC");
        ASSERT_TRUE(HasChild(fnC, "FnD"));
        ASSERT_EQ(1, fnC->m_children.size());
        ASSERT_EQ(1, fnC->m_parents.size());
        ASSERT_TRUE(HasParent(fnC, "FnA"));

        auto* fnD = GetFunction(fnC->m_children, "FnD");
        ASSERT_TRUE(HasChild(fnD, "FnB"));
        ASSERT_EQ(1, fnD->m_children.size());
        ASSERT_EQ(1, fnD->m_parents.size());

        ASSERT_TRUE(HasParent(fnD, "FnC"));
        ASSERT_FALSE(HasParent(fnD, "FnA"));

        auto* fnB = GetFunction(fnD->m_children, "FnB");
        ASSERT_EQ(0, fnB->m_children.size());
        ASSERT_EQ(1, fnB->m_parents.size());
        ASSERT_TRUE(HasParent(fnB, "FnD"));

        ASSERT_FALSE(HasParent(fnB, "FnA"));
        ASSERT_FALSE(HasChild(fnB, "FnA"));
    }

    TEST_F(SystemsSchedulerTest, AutomaticScheduling_WillChooseTheFirstFunctionAsDependantIfItsWeakDependencyIsNotInTheGraph)
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
        ShuffleFunctions(sut);

        // In this case, the order of registration actually matters as both FnA and FnB
        // so the first one in the list is going to be chosen as the child.
        std::string resolvedParent = "";
        for (const auto& function : sut.m_functions)
        {
            if (function.m_name == "FnA")
            {
                resolvedParent = "FnB";
                break;
            }

            if (function.m_name == "FnB")
            {
                resolvedParent = "FnA";
                break;
            }
        }

        sut.Prepare();
        ASSERT_EQ(1, sut.m_starterFunctions.size());

        auto* starterFn = sut.m_starterFunctions[0];
        ASSERT_EQ(resolvedParent, starterFn->m_name);

        if (resolvedParent == "FnA")
        {
            // In this case, the FnB function is first in the list of registered functions, so it will choose FnA as its parent.
            // Additionally, because FnA has one other child, after adding FnB as its new child, it will now have two children.

            ASSERT_EQ(0, starterFn->m_parents.size());
            ASSERT_EQ(2, starterFn->m_children.size());

            ASSERT_TRUE(HasChild(starterFn, "FnB"));
            ASSERT_TRUE(HasChild(starterFn, "FnC"));

            ASSERT_FALSE(HasParent(starterFn, "FnB"));

            ASSERT_EQ(0, starterFn->m_weakDependencies.size());

            auto* dependantFn = GetFunction(starterFn->m_children, "FnB");
            ASSERT_EQ(0, dependantFn->m_children.size());
            ASSERT_EQ(1, dependantFn->m_parents.size());
            ASSERT_TRUE(HasParent(dependantFn, resolvedParent));
        }
        else
        {
            // In this case, the FnA function is first in the list of registered functions, so it will choose FnB as its parent.
            // Additionally, FnB doesn't have any other children, so after adding FnA as its child, it will only have one child.

            ASSERT_EQ(0, starterFn->m_parents.size());
            ASSERT_EQ(1, starterFn->m_children.size());

            ASSERT_TRUE(HasChild(starterFn, "FnA"));
            ASSERT_FALSE(HasParent(starterFn, "FnA"));

            ASSERT_EQ(0, starterFn->m_weakDependencies.size());

            auto* dependantFn = GetFunction(starterFn->m_children, "FnA");
            ASSERT_EQ(1, dependantFn->m_children.size());
            ASSERT_EQ(1, dependantFn->m_parents.size());
            ASSERT_TRUE(HasParent(dependantFn, resolvedParent));
        }
    }

    // TODO: Add test case to make sure independent functions are put together into the starter functions.

    TEST_F(SystemsSchedulerTest, RegisterFunction_WillThrowIfAccessListHasSameComponentWithMultipleAccesses)
    {
        struct Functions
        {
            struct Component
            {};

            struct AccessList : be::BaseAccessList
            {
                using Get = be::Components<Component>;
                using Update = be::Components<Component>;
            };

            static void Run(const be::View<AccessList>&){};
        };

        SystemsSchedulerWIP sut{};

        ASSERT_THROW(sut.RegisterFunction("Functions::Run", Functions::Run), std::runtime_error);
    }
} // namespace be::tests::unit
