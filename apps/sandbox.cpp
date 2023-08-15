#include <Beast/EntryPoint.h>
#include <Beast/BeastEngine.h>
#include <Beast/Loggers/LoggersFactories.h>

#include <Beast/Ecs/Types.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/SystemsScheduler.h>

#include <entt/entt.hpp>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <spdlog/spdlog.h>

/*
* ECS DESIGN
* In entt, the single source of truth is the register.
* I want to have something similar, which will hide a dependency on that register internally. This will be called a world.
* class World{};
* 
* This class will act exactly like the entt's registry. It will hold all entities and all their components. It will also be used to create
* views with specific access lists.
* 
* So, basically, I don't want my systems to use anthing from the entt directly.
* Also, when I will be creating new entities before doing any scheduling, I will use world.
* I think, the View then can either accept the world, or maybe just the view implementation which will be the entt view?
* View has to accepts the entt view. I'm not sure how to pass it there correctly (meaning, so that we know its type, but we can figure this out).
* Unless we can just pass the registry there. Actually I can't see why it would be worse than passing the view? Maybe because it's then easier to
* modify the registry outside of where it should actually be modified (the world). So if I can find a way to pass just a view, that will be better.
* 
* Ok, so I decided that View will accept reference to the registry to make it simpler to implement. It's a wrapper around entt.
* That's my goal after all. I'm not creating my own ecs library, but rather create wrappers to hide it from my systems.
*/

struct ComponentA
{
    unsigned int data = 0;
};

struct ComponentB
{
    unsigned int data = 0;
};

class Attacher
{
public:
    struct AccessList : be::BaseAccessList
    {
        using Add = be::Components<ComponentA, ComponentB>;
    };

    void Run(const be::View<AccessList>& view)
    {
        for (auto entity : view)
        {
            view.AddComponent<ComponentA>(entity, {.data = entt::to_integral(entity)});
            view.AddComponent<ComponentB>(entity, {.data = entt::to_integral(entity)});
        }
    }
};

class MySystem
{
public:
    struct AccessList : be::BaseAccessList
    {
        using Get = be::Components<ComponentA>;
        using Update = be::Components<ComponentB>;
    };

    void Run(const be::View<AccessList>& view)
    {
        for (auto ent : view)
        {
            spdlog::info("Component {} for entity {} = {}", typeid(ComponentA).name(), static_cast<be::uint32>(ent), view.GetComponent<ComponentA>(ent).data);
            spdlog::info("Component {} for entity {} = {}", typeid(ComponentB).name(), static_cast<be::uint32>(ent), ++view.UpdateComponent<ComponentB>(ent).data);
            //std::cout << comp.data++ << "\n";
        }
        /*std::cout << view.Get<ComponentA>() << "\n";
        std::cout << view.Get<ComponentB>() << "\n";*/
    }
};

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig, const be::WindowDescriptor& windowDescriptor)
        : be::AApplication(std::move(engineConfig), windowDescriptor), m_logger(be::ConsoleLogger::Create("client_console_logger"))
    {
        m_window->SetWindowClosedEventHandler(OnWindowClosed());
        m_mouse->SetWheelScrolledListener(OnWheelScrolled());
    }

    void Run() override
    {
        GetEngine().PrintInfo();

        /*auto previousCords = m_mouse->GetMousePosition();
        const auto& currentCoords = m_mouse->GetMousePosition();

        while (m_isRunning)
        {
            if (previousCords != currentCoords)
            {
                m_logger->LogInfo("Mouse coords: [{}, {}]\n", currentCoords.x, currentCoords.y);
                previousCords = currentCoords;
            }

            m_window->ProcessInput();
            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_LEFT))
            {
                m_logger->LogInfo("Left  button pressed");
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_MIDDLE))
            {
                m_logger->LogInfo("Middle button pressed");
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_RIGHT))
            {
                m_logger->LogInfo("Right button pressed");
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON4))
            {
                m_logger->LogInfo("Button 4 pressed");
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON5))
            {
                m_logger->LogInfo("Button 5 pressed");
            }

            if (m_mouse->IsButtonHeldDown(be::MouseButtonCode::BUTTON_LEFT))
            {
                m_logger->LogInfo("Left button held down");
            }

            if (m_mouse->IsButtonHeldDown(be::MouseButtonCode::BUTTON_MIDDLE))
            {
                m_logger->LogInfo("Middle button held down");
            }

            if (m_mouse->IsButtonHeldDown(be::MouseButtonCode::BUTTON_RIGHT))
            {
                m_logger->LogInfo("Right button held down");
            }

            if (m_keyboard->IsKeyPressed(be::KeyCode::Right))
            {   
                m_logger->LogInfo("Right arrow pressed\n");
            }

            if (m_keyboard->IsKeyHeldDown(be::KeyCode::Right))
            {
                m_logger->LogInfo("Right arrow held down!\n");
            }

            if (m_keyboard->IsKeyDown(be::KeyCode::Right))
            {
                m_logger->LogInfo("Right arrow is down!\n");
            }

            if (m_keyboard->IsKeyPressed(be::KeyCode::Escape))
            {
                break;
            }
        }*/

        be::World world;
        be::SystemsScheduler scheduler(world);
        auto group = scheduler.CreateGroup();
        group.AttachSystem<MySystem>();
        //group.AttachSystem<MySystem>(1);

        scheduler.Prepare({group});

        [[maybe_unused]] const auto entity = world.CreateEntity();

        while (m_isRunning)
        {
            m_window->ProcessInput();
            if (m_keyboard->IsKeyPressed(be::KeyCode::Escape))
            {
                //scheduler.Abort();
                break;
            }

            scheduler.Update();
        }
    }

private:
    be::WindowClosedEventHandler OnWindowClosed()
    {
        return [&]() {
            m_isRunning = false;
        };
    }

    be::MouseWheelScrolledListener OnWheelScrolled()
    {
        return [](be::WheelScrollDirection direction) {
            std::cout << "Mouse scrolled " << (direction == be::WheelScrollDirection::SCROLL_UP ? "UP" : "DOWN") << "\n";
        };
    }

private:
    bool m_isRunning = true;
    const be::Shared<be::Logger> m_logger = nullptr;
};

be::Unique<be::AApplication> be::CreateApplication(WindowHandleInstance windowHandleInstance)
{
    // Configure engine
    auto config = be::EngineConfig();

    // Configure window
    be::WindowDescriptor windowDescriptor(std::move(windowHandleInstance));
    windowDescriptor.style = WindowStyle::WINDOW_DEFUALT;

    return be::MakeUnique<BasicApplication>(std::move(config), windowDescriptor);
}
