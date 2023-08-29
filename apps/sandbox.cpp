#include <Beast/EntryPoint.h>
#include <Beast/BeastEngine.h>
#include <Beast/Loggers/LoggersFactories.h>
#include <Beast/Common/Types.h>

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

#include <shared_mutex>
#include <mutex>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

struct ComponentA
{
    unsigned int data = 0;
};

struct ComponentB
{
    unsigned int data = 0;
};

struct Input
{
    be::Mouse& mouse;
    be::Keyboard& keyboard;
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

class Getter
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
        }
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

        auto& scheduler = m_ecs.scheduler;

        auto group1 = scheduler.CreateGroup();
        group1.AttachSystem<Getter>();

        auto group2 = scheduler.CreateGroup();
        group2.AttachSystem<Attacher>();

        scheduler.Prepare({group2, group1});
        m_ecs.world.CreateEntity();

        auto previousCords = m_mouse->GetMousePosition();
        const auto& currentCoords = m_mouse->GetMousePosition();

        while (m_isRunning)
        {
            if (previousCords != currentCoords)
            {
                m_logger->LogInfo("Mouse coords: [{}, {}]\n", currentCoords.x, currentCoords.y);
                previousCords = currentCoords;
            }

            m_window->ProcessInput();

            if (m_keyboard->IsKeyPressed(be::KeyCode::Escape))
            {
                break;
            }

            scheduler.Update();
        }
    }

private:
    be::WindowClosedEventHandler
        OnWindowClosed()
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
