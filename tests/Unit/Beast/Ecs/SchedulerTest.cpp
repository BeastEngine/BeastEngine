#include <Beast/Ecs/Scheduler.h>
#include <Beast/Ecs/SystemFunction.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/Components/Graphics.h>
#include <Beast/Ecs/SystemsRunner.h>

#include <gtest/gtest.h>
#include <fmt/format.h>

#include <span>
#include <string_view>
#include <random>
#include <thread>
#include <atomic>

namespace be::tests::unit
{
    static void RegisterShuffled(std::vector<std::function<const char*()>> registrators)
    {
        // Shuffle the functions to make sure the order of registration doesn't matter
        std::random_device rd{};
        std::default_random_engine rng{rd()};
        std::shuffle(registrators.begin(), registrators.end(), rng);

        for (size_t i = 0; i < registrators.size(); ++i)
        {
            const auto& registrator = registrators[i];
            std::cout << fmt::format("Function {} {}\n", i, registrator());
        }
    }

    class SchedulerTest : public testing::Test
    {
    };

#define BE_SINGLE_COMPONENT_AL(Type)                \
    struct AL##Type : be::BaseAccessList            \
    {                                               \
        using Type = be::Components<be::Transform>; \
    };

    static const SystemFunction* GetFunction(std::span<const SystemFunction* const> source, std::string_view functionToFind)
    {
        const auto foundIt = std::find_if(source.begin(), source.end(), [functionToFind](const SystemFunction* function) {
            return function->Name() == functionToFind;
        });

        return foundIt == source.end() ? nullptr : *foundIt;
    }

    static bool HasChild(const SystemFunction* function, std::string_view expectedChild)
    {
        return GetFunction(function->Children(), expectedChild) != nullptr;
    }

    static bool HasParent(const SystemFunction* function, std::string_view expectedParent)
    {
        return GetFunction(function->Parents(), expectedParent) != nullptr;
    }

    TEST_F(SchedulerTest, AutomaticScheduling)
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

        Scheduler sut{};
        RegisterShuffled({
            [&]() { sut.RegisterFunction("getFunction", Functions::getFunction); return "getFunction"; },
            [&]() { sut.RegisterFunction("updateFunction", Functions::updateFunction); return "updateFunction"; },
            [&]() { sut.RegisterFunction("addFunction", Functions::addFunction); return "addFunction"; },
            [&]() { sut.RegisterFunction("removeFunction", Functions::removeFunction); return "removeFunction"; },
        });

        const auto schedule = sut.Prepare();

        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(1, starterFunctions.size());
        auto* starterFn = starterFunctions[0];
        ASSERT_EQ("removeFunction", starterFn->Name());
        ASSERT_EQ(0, starterFn->Parents().size());
        ASSERT_EQ(1, starterFn->Children().size());

        ASSERT_TRUE(HasChild(starterFn, "addFunction"));

        const auto* addFn = GetFunction(starterFn->Children(), "addFunction");
        ASSERT_EQ(1, addFn->Parents().size());
        ASSERT_EQ(1, addFn->Children().size());

        ASSERT_TRUE(HasParent(addFn, "removeFunction"));
        ASSERT_TRUE(HasChild(addFn, "updateFunction"));

        const auto* updateFn = GetFunction(addFn->Children(), "updateFunction");
        ASSERT_EQ(1, updateFn->Parents().size());
        ASSERT_EQ(1, updateFn->Children().size());

        ASSERT_TRUE(HasParent(updateFn, "addFunction"));
        ASSERT_TRUE(HasChild(updateFn, "getFunction"));

        const auto* getFn = GetFunction(updateFn->Children(), "getFunction");
        ASSERT_EQ(1, updateFn->Parents().size());
        ASSERT_TRUE(getFn->Children().empty());

        ASSERT_TRUE(HasParent(getFn, "updateFunction"));
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Standalone_WillThrowWhenTwoFunctionsStronglyDependOnEachOther)
    {
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

        Scheduler sut{};
        RegisterShuffled({
            [&]() { sut.RegisterFunction("functionA", Functions::FunctionA); return "functionA"; },
            [&]() { sut.RegisterFunction("functionB", Functions::FunctionB); return "functionB"; },
        });

        ASSERT_THROW(sut.Prepare(), std::runtime_error);
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Standalone_WillThrowWhenThereIsCircularDependencyInGraph)
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

        Scheduler sut{};
        RegisterShuffled({
            [&] {sut.RegisterFunction("StarterFn", Functions::StarterFn); return "StarterFn"; },
            [&] { sut.RegisterFunction("FnA", Functions::FnA); return "FnA"; },
            [&] { sut.RegisterFunction("FnB", Functions::FnB); return "FnB"; },
            [&] { sut.RegisterFunction("FnC", Functions::FnC); return "FnC"; },
        });

        ASSERT_THROW(sut.Prepare(), std::runtime_error);
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Standalone_RealWorldScenario)
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

        Scheduler sut{};
        RegisterShuffled({
            [&] {sut.RegisterFunction("PlayerSpawner", Functions::PlayerSpawner); return "PlayerSpawner"; },
            [&] { sut.RegisterFunction("PlayerMover", Functions::PlayerMover); return "PlayerMover"; },
            [&] { sut.RegisterFunction("BulletMover", Functions::BulletMover); return "BulletMover"; },
            [&] { sut.RegisterFunction("PlayerShooter", Functions::PlayerShooter); return "PlayerShooter"; },
        });
        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

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

        ASSERT_EQ(1, starterFunctions.size());
        auto* starterFn = starterFunctions[0];
        ASSERT_EQ("PlayerSpawner", starterFn->Name());
        ASSERT_EQ(0, starterFn->Parents().size());
        ASSERT_EQ(1, starterFn->Children().size());

        ASSERT_TRUE(HasChild(starterFn, "PlayerMover"));

        const auto* playerMover = GetFunction(starterFn->Children(), "PlayerMover");
        ASSERT_EQ(1, playerMover->Children().size());
        ASSERT_EQ(1, playerMover->Parents().size());

        ASSERT_TRUE(HasParent(playerMover, "PlayerSpawner"));
        ASSERT_TRUE(HasChild(playerMover, "PlayerShooter"));

        const auto* playerShooter = GetFunction(playerMover->Children(), "PlayerShooter");

        ASSERT_EQ(1, playerShooter->Children().size());
        ASSERT_EQ(1, playerShooter->Parents().size());

        ASSERT_TRUE(HasParent(playerShooter, "PlayerMover"));

        ASSERT_TRUE(HasChild(playerShooter, "BulletMover"));

        const auto* bulletMover = GetFunction(playerShooter->Children(), "BulletMover");

        ASSERT_EQ(1, bulletMover->Parents().size());
        ASSERT_EQ(0, bulletMover->Children().size());

        ASSERT_TRUE(HasParent(bulletMover, "PlayerShooter"));
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Standalone_WillResolveWeakDependencyViaIndirectStrongDependency)
    {
        struct Functions
        {
            struct Component
            {};

            struct FnAAccessList : be::BaseAccessList
            {
                using Add = be::Components<be::Transform, Component>;
            };
            static void FnA(const be::View<FnAAccessList>&) {};

            struct FnBAccessList : be::BaseAccessList
            {
                using Add = be::Components<Component>;
                using Get = be::Components<be::Sprite>;
            };
            static void FnB(const be::View<FnBAccessList>&) {};

            struct FnCAccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
            };
            static void FnC(const be::View<FnCAccessList>&) {};

            struct FnDAccessList : be::BaseAccessList
            {
                using Get = be::Components<be::Transform>;
                using Add = be::Components<be::Sprite>;
            };
            static void FnD(const be::View<FnDAccessList>&) {};
        };

        Scheduler sut{};
        RegisterShuffled({
            [&] {sut.RegisterFunction("FnD", Functions::FnD); return "FnD"; },
            [&] { sut.RegisterFunction("FnA", Functions::FnA); return "FnA"; },
            [&] { sut.RegisterFunction("FnC", Functions::FnC); return "FnC"; },
            [&] { sut.RegisterFunction("FnB", Functions::FnB); return "FnB"; },
        });

        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(1, starterFunctions.size());

        auto* starterFn = starterFunctions[0];
        ASSERT_EQ("FnA", starterFn->Name());

        ASSERT_EQ(0, starterFn->Parents().size());
        ASSERT_EQ(1, starterFn->Children().size());

        ASSERT_TRUE(HasChild(starterFn, "FnC"));

        ASSERT_FALSE(HasChild(starterFn, "FnB"));
        ASSERT_FALSE(HasParent(starterFn, "FnB"));

        auto* fnC = GetFunction(starterFn->Children(), "FnC");
        ASSERT_TRUE(HasChild(fnC, "FnD"));
        ASSERT_EQ(1, fnC->Children().size());
        ASSERT_EQ(1, fnC->Parents().size());
        ASSERT_TRUE(HasParent(fnC, "FnA"));

        auto* fnD = GetFunction(fnC->Children(), "FnD");
        ASSERT_TRUE(HasChild(fnD, "FnB"));
        ASSERT_EQ(1, fnD->Children().size());
        ASSERT_EQ(1, fnD->Parents().size());

        ASSERT_TRUE(HasParent(fnD, "FnC"));
        ASSERT_FALSE(HasParent(fnD, "FnA"));

        auto* fnB = GetFunction(fnD->Children(), "FnB");
        ASSERT_EQ(0, fnB->Children().size());
        ASSERT_EQ(1, fnB->Parents().size());
        ASSERT_TRUE(HasParent(fnB, "FnD"));

        ASSERT_FALSE(HasParent(fnB, "FnA"));
        ASSERT_FALSE(HasChild(fnB, "FnA"));
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Standalone_WillChooseTheFirstFunctionAsDependantIfItsWeakDependencyIsNotInTheGraph)
    {
        struct Functions
        {
            struct Component
            {};

            struct FnAAccessList : be::BaseAccessList
            {
                using Add = be::Components<be::Transform, Component>;
            };
            static void FnA(const be::View<FnAAccessList>&) {};

            struct FnBAccessList : be::BaseAccessList
            {
                using Add = be::Components<Component>;
            };
            static void FnB(const be::View<FnBAccessList>&) {};

            struct FnCAccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
            };
            static void FnC(const be::View<FnCAccessList>&) {};

            struct FnDAccessList : be::BaseAccessList
            {
                using Get = be::Components<be::Transform>;
                using Add = be::Components<be::Sprite>;
            };
            static void FnD(const be::View<FnDAccessList>&) {};
        };

        Scheduler sut{};

        RegisterShuffled({
            [&] { sut.RegisterFunction("FnA", Functions::FnA); return "FnA"; },
            [&] { sut.RegisterFunction("FnB", Functions::FnB); return "FnB"; },
            [&] { sut.RegisterFunction("FnC", Functions::FnC); return "FnC"; },
            [&] {sut.RegisterFunction("FnD", Functions::FnD); return "FnD"; },
        });

        // In this case, the order of registration actually matters as both FnA and FnB
        // so the first one in the list is going to be chosen as the child.

        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(1, starterFunctions.size());

        auto* starterFn = starterFunctions[0];
        ASSERT_TRUE(starterFn->Name() == "FnA" || starterFn->Name() == "FnB");

        if (starterFn->Name() == "FnA")
        {
            // In this case, the FnB function is first in the list of registered functions, so it will choose FnA as its parent.
            // Additionally, because FnA has one other child, after adding FnB as its new child, it will now have two children.

            ASSERT_EQ(0, starterFn->Parents().size());
            ASSERT_EQ(2, starterFn->Children().size());

            ASSERT_TRUE(HasChild(starterFn, "FnB"));
            ASSERT_TRUE(HasChild(starterFn, "FnC"));

            ASSERT_FALSE(HasParent(starterFn, "FnB"));

            auto* dependantFn = GetFunction(starterFn->Children(), "FnB");
            ASSERT_EQ(0, dependantFn->Children().size());
            ASSERT_EQ(1, dependantFn->Parents().size());
            ASSERT_TRUE(HasParent(dependantFn, "FnA"));
        }
        else
        {
            // In this case, the FnA function is first in the list of registered functions, so it will choose FnB as its parent.
            // Additionally, FnB doesn't have any other children, so after adding FnA as its child, it will only have one child.

            ASSERT_EQ(0, starterFn->Parents().size());
            ASSERT_EQ(1, starterFn->Children().size());

            ASSERT_TRUE(HasChild(starterFn, "FnA"));
            ASSERT_FALSE(HasParent(starterFn, "FnA"));

            auto* dependantFn = GetFunction(starterFn->Children(), "FnA");
            ASSERT_EQ(1, dependantFn->Children().size());
            ASSERT_EQ(1, dependantFn->Parents().size());
            ASSERT_TRUE(HasParent(dependantFn, "FnB"));
        }
    }

    TEST_F(SchedulerTest, RegisterFunction_Standalone_WillThrowIfAccessListHasSameComponentWithMultipleAccesses)
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

            static void Run(const be::View<AccessList>&) {};
        };

        Scheduler sut{};

        ASSERT_THROW(sut.RegisterFunction("Functions::Run", Functions::Run), std::runtime_error);
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Standalone_WillPutUnrelatedFunctionsIntoStarterFunctions)
    {
        struct Functions
        {
            struct ComponentA
            {};

            struct ComponentB
            {};

            struct AccessListA : be::BaseAccessList
            {
                using Add = be::Components<ComponentA>;
            };

            static void FnA(const be::View<AccessListA>&) {}

            struct AccessListB : be::BaseAccessList
            {
                using Add = be::Components<ComponentB>;
            };

            static void FnB(const be::View<AccessListB>&) {}
        };

        Scheduler sut{};

        sut.RegisterFunction("FnA", Functions::FnA);
        sut.RegisterFunction("FnB", Functions::FnB);

        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(2, starterFunctions.size());

        const auto* fnA = GetFunction(starterFunctions, "FnA");
        ASSERT_EQ(0, fnA->Children().size());
        ASSERT_EQ(0, fnA->Parents().size());

        const auto* fnB = GetFunction(starterFunctions, "FnB");
        ASSERT_EQ(0, fnB->Children().size());
        ASSERT_EQ(0, fnB->Parents().size());
    }

    TEST_F(SchedulerTest, RegisterFunctions_Standalone_WillThrowIfFunctionWithGivenNameAlreadyRegistered)
    {
        struct Functions
        {
            struct ComponentA
            {};

            struct ComponentB
            {};

            struct AccessListA : be::BaseAccessList
            {
                using Add = be::Components<ComponentA>;
            };

            static void FnA(const be::View<AccessListA>&) {}

            struct AccessListB : be::BaseAccessList
            {
                using Add = be::Components<ComponentB>;
            };

            static void FnB(const be::View<AccessListB>&) {}
        };

        Scheduler sut{};

        ASSERT_NO_THROW(sut.RegisterFunction("FnA", Functions::FnA));
        ASSERT_THROW(sut.RegisterFunction("FnA", Functions::FnB), std::runtime_error);
    }

    /*******************************************************
     *******************************************************
     * TEST WITH MEMBER FUNCTIONS.
     *******************************************************
     *******************************************************
     */

    TEST_F(SchedulerTest, AutomaticScheduling_Member_WillThrowWhenTwoFunctionsStronglyDependOnEachOther)
    {
        struct Component1
        {
        };

        struct SystemA
        {
            struct AAL : be::BaseAccessList
            {
                using Update = be::Components<Component1>;
                using Get = be::Components<be::Transform>;
            };
            void Run(const be::View<AAL>&) {}
        } sysA;

        struct SystemB
        {
            struct BAL : be::BaseAccessList
            {
                using Get = be::Components<Component1>;
                using Update = be::Components<be::Transform>;
            };
            void Run(const be::View<BAL>&) {}
        } sysB;

        Scheduler sut{};

        sut.RegisterFunction("SystemA::run", &SystemA::Run, &sysA);
        sut.RegisterFunction("SystemB::run", &SystemB::Run, &sysB);

        ASSERT_THROW(sut.Prepare(), std::runtime_error);
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Member_WillThrowWhenThereIsCircularDependencyInGraph)
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

        struct StarterSystem
        {
            struct StarterAL : be::BaseAccessList
            {
                using Add = be::Components<be::Transform>;
            };
            void Run(const be::View<StarterAL>&) {}
        } starter;

        struct SystemA
        {
            struct AAL : be::BaseAccessList
            {
                using Add = be::Components<Component1>;
                using Get = be::Components<Component2, be::Transform>;
            };
            void Run(const be::View<AAL>&) {}
        } sysA;

        struct SystemB
        {
            struct BAL : be::BaseAccessList
            {
                using Add = be::Components<Component2>;
                using Get = be::Components<Component3>;
            };
            void Run(const be::View<BAL>&) {}
        } sysB;

        struct SystemC
        {
            struct CAL : be::BaseAccessList
            {
                using Add = be::Components<Component3>;
                using Get = be::Components<Component1>;
            };
            void Run(const be::View<CAL>&) {}
        } sysC;

        Scheduler sut{};
        RegisterShuffled({
            [&] {sut.RegisterFunction("StarterSystem::Run", &StarterSystem::Run, &starter); return "StarterSystem::Run"; },
            [&] { sut.RegisterFunction("SystemA::Run", &SystemA::Run, &sysA); return "SystemA::Run"; },
            [&] { sut.RegisterFunction("SystemB::Run", &SystemB::Run, &sysB); return "SystemB::Run"; },
            [&] { sut.RegisterFunction("SystemC::Run", &SystemC::Run, &sysC); return "SystemC::Run"; },
        });

        ASSERT_THROW(sut.Prepare(), std::runtime_error);
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Member_RealWorldScenario)
    {
        struct Player
        {};

        struct Mover
        {};

        struct Bullet
        {};

        struct PlayerSystem
        {
            struct PlayerSpawnerAL : be::BaseAccessList
            {
                using Add = be::Components<Player, Mover, be::Transform, be::Sprite>;
            };
            void PlayerSpawner(const be::View<PlayerSpawnerAL>&) {}

            struct PlayerMoverAL : be::BaseAccessList
            {
                using Update = be::Components<Mover, be::Transform>;
                using Get = be::Components<Player>;
            };
            void PlayerMover(const be::View<PlayerMoverAL>&) {}

            struct SpawnBulletsAL : be::BaseAccessList
            {
                using Add = be::Components<Bullet, be::Sprite>;
                using Update = be::Components<be::Transform>;
                using Get = be::Components<Player, Mover>;
            };
            void PlayerShooter(const be::View<SpawnBulletsAL>&) {}
        } playerSys;

        struct BulletMover
        {
            struct BulletMoverAL : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
                using Get = be::Components<Bullet>;
            };
            void Run(const be::View<BulletMoverAL>&) {}
        } bulletSys;

        Scheduler sut{};
        RegisterShuffled({
            [&] {sut.RegisterFunction("PlayerSystem::PlayerSpawner", &PlayerSystem::PlayerSpawner, &playerSys); return "PlayerSystem::PlayerSpawner"; },
            [&] { sut.RegisterFunction("PlayerSystem::PlayerMover", &PlayerSystem::PlayerMover, &playerSys); return "PlayerSystem::PlayerMover"; },
            [&] { sut.RegisterFunction("BulletMover::Run", &BulletMover::Run, &bulletSys); return "BulletMover::Run"; },
            [&] { sut.RegisterFunction("PlayerSystem::PlayerShooter", &PlayerSystem::PlayerShooter, &playerSys); return "PlayerSystem::PlayerShooter"; },
        });

        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

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

        ASSERT_EQ(1, starterFunctions.size());
        auto* starterFn = starterFunctions[0];
        ASSERT_EQ("PlayerSystem::PlayerSpawner", starterFn->Name());
        ASSERT_EQ(0, starterFn->Parents().size());
        ASSERT_EQ(1, starterFn->Children().size());

        ASSERT_TRUE(HasChild(starterFn, "PlayerSystem::PlayerMover"));

        const auto* playerMover = GetFunction(starterFn->Children(), "PlayerSystem::PlayerMover");
        ASSERT_EQ(1, playerMover->Children().size());
        ASSERT_EQ(1, playerMover->Parents().size());

        ASSERT_TRUE(HasParent(playerMover, "PlayerSystem::PlayerSpawner"));
        ASSERT_TRUE(HasChild(playerMover, "PlayerSystem::PlayerShooter"));

        const auto* playerShooter = GetFunction(playerMover->Children(), "PlayerSystem::PlayerShooter");

        ASSERT_EQ(1, playerShooter->Children().size());
        ASSERT_EQ(1, playerShooter->Parents().size());

        ASSERT_TRUE(HasParent(playerShooter, "PlayerSystem::PlayerMover"));

        ASSERT_TRUE(HasChild(playerShooter, "BulletMover::Run"));

        const auto* bulletMover = GetFunction(playerShooter->Children(), "BulletMover::Run");

        ASSERT_EQ(1, bulletMover->Parents().size());
        ASSERT_EQ(0, bulletMover->Children().size());

        ASSERT_TRUE(HasParent(bulletMover, "PlayerSystem::PlayerShooter"));
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Member_WillResolveWeakDependencyViaIndirectStrongDependency)
    {
        struct Component
        {};

        struct SystemA
        {
            struct FnAAccessList : be::BaseAccessList
            {
                using Add = be::Components<be::Transform, Component>;
            };
            void Run(const be::View<FnAAccessList>&) {};
        } sysA;

        struct SystemB
        {
            struct FnBAccessList : be::BaseAccessList
            {
                using Add = be::Components<Component>;
                using Get = be::Components<be::Sprite>;
            };
            void Run(const be::View<FnBAccessList>&) {};
        } sysB;

        struct SystemC
        {
            struct FnCAccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
            };
            void Run(const be::View<FnCAccessList>&) {};
        } sysC;

        struct SystemD
        {
            struct FnDAccessList : be::BaseAccessList
            {
                using Get = be::Components<be::Transform>;
                using Add = be::Components<be::Sprite>;
            };
            void Run(const be::View<FnDAccessList>&) {};
        } sysD;

        Scheduler sut{};
        RegisterShuffled({
            [&] {sut.RegisterFunction("SystemD::Run", &SystemD::Run, &sysD); return "SystemD::Run"; },
            [&] { sut.RegisterFunction("SystemA::Run", &SystemA::Run, &sysA); return "SystemA::Run"; },
            [&] { sut.RegisterFunction("SystemC::Run", &SystemC::Run, &sysC); return "SystemC::Run"; },
            [&] { sut.RegisterFunction("SystemB::Run", &SystemB::Run, &sysB); return "SystemB::Run"; },
        });

        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(1, starterFunctions.size());

        auto* starterFn = starterFunctions[0];
        ASSERT_EQ("SystemA::Run", starterFn->Name());

        ASSERT_EQ(0, starterFn->Parents().size());
        ASSERT_EQ(1, starterFn->Children().size());

        ASSERT_TRUE(HasChild(starterFn, "SystemC::Run"));

        ASSERT_FALSE(HasChild(starterFn, "SystemB::Run"));
        ASSERT_FALSE(HasParent(starterFn, "SystemB::Run"));

        auto* fnC = GetFunction(starterFn->Children(), "SystemC::Run");
        ASSERT_TRUE(HasChild(fnC, "SystemD::Run"));
        ASSERT_EQ(1, fnC->Children().size());
        ASSERT_EQ(1, fnC->Parents().size());
        ASSERT_TRUE(HasParent(fnC, "SystemA::Run"));

        auto* fnD = GetFunction(fnC->Children(), "SystemD::Run");
        ASSERT_TRUE(HasChild(fnD, "SystemB::Run"));
        ASSERT_EQ(1, fnD->Children().size());
        ASSERT_EQ(1, fnD->Parents().size());

        ASSERT_TRUE(HasParent(fnD, "SystemC::Run"));
        ASSERT_FALSE(HasParent(fnD, "SystemA::Run"));

        auto* fnB = GetFunction(fnD->Children(), "SystemB::Run");
        ASSERT_EQ(0, fnB->Children().size());
        ASSERT_EQ(1, fnB->Parents().size());
        ASSERT_TRUE(HasParent(fnB, "SystemD::Run"));

        ASSERT_FALSE(HasParent(fnB, "SystemA::Run"));
        ASSERT_FALSE(HasChild(fnB, "SystemA::Run"));
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Member_WillChooseTheFirstFunctionAsDependantIfItsWeakDependencyIsNotInTheGraph)
    {
        struct Component
        {};

        struct SystemA
        {
            struct FnAAccessList : be::BaseAccessList
            {
                using Add = be::Components<be::Transform, Component>;
            };
            void Run(const be::View<FnAAccessList>&) {};
        } sysA;

        struct SystemB
        {
            struct FnBAccessList : be::BaseAccessList
            {
                using Add = be::Components<Component>;
            };
            void Run(const be::View<FnBAccessList>&) {};
        } sysB;

        struct SystemC
        {
            struct FnCAccessList : be::BaseAccessList
            {
                using Update = be::Components<be::Transform>;
            };
            void Run(const be::View<FnCAccessList>&) {};
        } sysC;

        struct SystemD
        {
            struct FnDAccessList : be::BaseAccessList
            {
                using Get = be::Components<be::Transform>;
                using Add = be::Components<be::Sprite>;
            };
            void Run(const be::View<FnDAccessList>&) {};
        } sysD;

        Scheduler sut{};
        RegisterShuffled({
            [&] { sut.RegisterFunction("SystemA::Run", &SystemA::Run, &sysA); return "SystemA::Run"; },
            [&] { sut.RegisterFunction("SystemB::Run", &SystemB::Run, &sysB); return "SystemB::Run"; },
            [&] { sut.RegisterFunction("SystemC::Run", &SystemC::Run, &sysC); return "SystemC::Run"; },
            [&] {sut.RegisterFunction("SystemD::Run", &SystemD::Run, &sysD); return "SystemD::Run"; },
        });

        // In this case, the order of registration actually matters as both FnA and FnB
        // so the first one in the list is going to be chosen as the child.
        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(1, starterFunctions.size());

        auto* starterFn = starterFunctions[0];
        ASSERT_TRUE(starterFn->Name() == "SystemA::Run" || starterFn->Name() == "SystemB::Run");

        if (starterFn->Name() == "SystemA::Run")
        {
            // In this case, the SystemB::Run function is first in the list of registered functions, so it will choose SystemA::Run as its parent.
            // Additionally, because SystemA::Run has one other child, after adding SystemB::Run as its new child, it will now have two children.

            ASSERT_EQ(0, starterFn->Parents().size());
            ASSERT_EQ(2, starterFn->Children().size());

            ASSERT_TRUE(HasChild(starterFn, "SystemB::Run"));
            ASSERT_TRUE(HasChild(starterFn, "SystemC::Run"));

            ASSERT_FALSE(HasParent(starterFn, "SystemB::Run"));

            auto* dependantFn = GetFunction(starterFn->Children(), "SystemB::Run");
            ASSERT_EQ(0, dependantFn->Children().size());
            ASSERT_EQ(1, dependantFn->Parents().size());
            ASSERT_TRUE(HasParent(dependantFn, "SystemA::Run"));
        }
        else
        {
            // In this case, the SystemA::Run function is first in the list of registered functions, so it will choose SystemB::Run as its parent.
            // Additionally, SystemB::Run doesn't have any other children, so after adding SystemA::Run as its child, it will only have one child.

            ASSERT_EQ(0, starterFn->Parents().size());
            ASSERT_EQ(1, starterFn->Children().size());

            ASSERT_TRUE(HasChild(starterFn, "SystemA::Run"));
            ASSERT_FALSE(HasParent(starterFn, "SystemA::Run"));

            auto* dependantFn = GetFunction(starterFn->Children(), "SystemA::Run");
            ASSERT_EQ(1, dependantFn->Children().size());
            ASSERT_EQ(1, dependantFn->Parents().size());
            ASSERT_TRUE(HasParent(dependantFn, "SystemB::Run"));
        }
    }

    TEST_F(SchedulerTest, RegisterFunction_Member_WillThrowIfAccessListHasSameComponentWithMultipleAccesses)
    {
        struct Component
        {};

        struct System
        {
            struct AccessList : be::BaseAccessList
            {
                using Get = be::Components<Component>;
                using Update = be::Components<Component>;
            };

            void Run(const be::View<AccessList>&) {};
        } sys;

        Scheduler sut{};
        ASSERT_THROW(sut.RegisterFunction("System::Run", &System::Run, &sys), std::runtime_error);
    }

    TEST_F(SchedulerTest, AutomaticScheduling_Member_WillPutUnrelatedFunctionsIntoStarterFunctions)
    {
        struct ComponentA
        {};

        struct ComponentB
        {};

        struct SystemA
        {
            struct AccessList : be::BaseAccessList
            {
                using Add = be::Components<ComponentA>;
            };

            void Run(const be::View<AccessList>&) {}
        } sysA;

        struct SystemB
        {
            struct AccessList : be::BaseAccessList
            {
                using Add = be::Components<ComponentB>;
            };

            void Run(const be::View<AccessList>&) {}
        } sysB;

        Scheduler sut{};

        sut.RegisterFunction("SystemA::Run", &SystemA::Run, &sysA);
        sut.RegisterFunction("SystemB::Run", &SystemB::Run, &sysB);

        const auto schedule = sut.Prepare();
        const auto& starterFunctions = schedule.starterFunctions;

        ASSERT_EQ(2, starterFunctions.size());

        const auto* fnA = GetFunction(starterFunctions, "SystemA::Run");
        ASSERT_EQ(0, fnA->Children().size());
        ASSERT_EQ(0, fnA->Parents().size());

        const auto* fnB = GetFunction(starterFunctions, "SystemB::Run");
        ASSERT_EQ(0, fnB->Children().size());
        ASSERT_EQ(0, fnB->Parents().size());
    }

    TEST_F(SchedulerTest, RegisterFunctions_Member_WillThrowIfFunctionWithGivenNameAlreadyRegistered)
    {
        struct ComponentA
        {};

        struct ComponentB
        {};

        struct SystemA
        {
            struct AccessList : be::BaseAccessList
            {
                using Add = be::Components<ComponentA>;
            };

            void Run(const be::View<AccessList>&) {}
        } sysA;

        struct SystemB
        {
            struct AccessList : be::BaseAccessList
            {
                using Add = be::Components<ComponentB>;
            };

            void Run(const be::View<AccessList>&) {}
        } sysB;

        Scheduler sut{};

        ASSERT_NO_THROW(sut.RegisterFunction("SystemA::Run", &SystemA::Run, &sysA));
        ASSERT_THROW(sut.RegisterFunction("SystemA::Run", &SystemB::Run, &sysB), std::runtime_error);
    }

    struct SystemRun
    {
        std::chrono::milliseconds timestamp;
        std::atomic_bool isRunning = false;
    };

    struct SystemInputParams
    {
        std::mutex& mutex;
        uint16& count;
        SystemRun& systemRun;
    };

    class TestSystem
    {
    public:
        TestSystem(SystemInputParams params)
            : m_params(std::move(params))
        {}

    protected:
        void RunImpl()
        {
            ASSERT_FALSE(m_params.systemRun.isRunning);
            m_params.systemRun.isRunning = true;

            ASSERT_TRUE(m_params.mutex.try_lock());
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            ++m_params.count;
            m_params.mutex.unlock();
        }

    private:
        SystemInputParams m_params;
    };

#define TEST_SYSTEM(Access)                  \
    class System##Access : public TestSystem \
    {                                        \
    public:                                  \
        void Run(const View<AL##Access>&)    \
        {                                    \
            RunImpl();                       \
        }                                    \
    }

    TEST_F(SchedulerTest, Schedule_Run_WillRunScheduledFunctionsInRightOrder)
    {
        std::mutex systemsMutex;
        uint16 count = 0;
        std::unordered_map<std::string, SystemRun> systemsRun;

        struct ALAdd : BaseAccessList
        {
            using Add = Components<Transform>;
        };

        struct ALUpdate : BaseAccessList
        {
            using Update = Components<Transform>;
        };

        struct ALGet : BaseAccessList
        {
            using Get = Components<Transform>;
        };

        struct ALRemove : BaseAccessList
        {
            using Remove = Components<Transform>;
        };

        constexpr auto SYSTEM_NAME_ADD = "SystemAdd::Run";
        constexpr auto SYSTEM_NAME_UPDATE = "SystemUpdate::Run";
        constexpr auto SYSTEM_NAME_GET = "SystemGet::Run";
        constexpr auto SYSTEM_NAME_REMOVE = "SystemRemove::Run";

        TEST_SYSTEM(Add)
        sysAdd(SystemInputParams{systemsMutex, count, systemsRun[SYSTEM_NAME_ADD]});

        TEST_SYSTEM(Update)
        sysUpdate(SystemInputParams{systemsMutex, count, systemsRun[SYSTEM_NAME_UPDATE]});

        TEST_SYSTEM(Get)
        sysGet(SystemInputParams{systemsMutex, count, systemsRun[SYSTEM_NAME_GET]});

        TEST_SYSTEM(Remove)
        sysRemove(SystemInputParams{systemsMutex, count, systemsRun[SYSTEM_NAME_REMOVE]});

        Scheduler scheduler{};
        scheduler.RegisterFunction(SYSTEM_NAME_ADD, &SystemAdd::Run, &sysAdd);
        scheduler.RegisterFunction(SYSTEM_NAME_UPDATE, &SystemUpdate::Run, &sysUpdate);
        scheduler.RegisterFunction(SYSTEM_NAME_GET, &SystemGet::Run, &sysGet);
        scheduler.RegisterFunction(SYSTEM_NAME_REMOVE, &SystemRemove::Run, &sysRemove);

        World world{};
        SystemsRunner runner{world, scheduler.Prepare()};
        runner.Run();

        ASSERT_EQ(4, count);
    }
} // namespace be::tests::unit