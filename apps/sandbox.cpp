#include <Beast/Core/EntryPoint.h>
#include <Beast/Core/BeastEngine.h>
#include <Beast/Core/Loggers/LoggersFactories.h>

#include <entt/entt.hpp>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <tuple>
#include <type_traits>
#include <spdlog/spdlog.h>

/*
* This would create two scheduling groups, that would we executed sequentialy, but the functions inside each groups, would be executed in parallel.
* Wouldn't it be easier to actually do it automatically?
* 
* Something like this. We would create the ViewDescriptor which would contain list of components that can be Get, Update, Assign, Remove.
* Those would affect the scheduling.
* In particular:
* We could go through all functions and:
* Take all functions that assign specific components. Those should definitelly be placed in a first scheduling group.
* So, we now go through all of them and see which components we assign.
* 
* Well, with manual scheduling all we have to do is know what data we need. So we can still use ViewDescriptor to filter components when creating views.
* Because I don't want to have to specify it manually as part of the function declariation.
* The problem with manual scheduling though, is that we can't hide anything inside the cpp file. Everything has to be available.
* Unless we will schedule by systems, instead of functions. In that case, each system would be responsible for a single functionality.
* This would mean a lot of systems, but it would also make it easier to schedule. Because we would know exactly what is needed by that system.
* So I think what's what im gonna do. Each system will be responsible for a single thing. It will also define it's access list
* which will be both hint for me which components are used, but also for the supplied view, on which components can be access and so on.
* 
* So, something like this:
* class MySystem : public be::System
* {
* public:
*   struct AccessList : be::AccessList
*   {
*       using Get = be::Components<Transform, Controller>;
*       using Update = be::Components<...>
*       ...
*   }
* 
*   void Update(const be::View<AccessList>& view) override;
* }
* 
* Then, somewhere in the view
* template<typename AccessList>
* class View
* {
* private:
*   using AL = AccessList;

* public:
*   template<typename Component>
*   const auto&& GetComponent(Entity entity)
*   {
*       // Seems very slow
*       if (!m_accesses[ToId<Component>()] == AccessType::GET)
*       {
*           throw;
*       }
* 
*   }
* 
* private:
*   std::unordered_map<type_info, AccessType> m_accesses;
* }
*/

struct ComponentA
{
    unsigned int data = 0;
};

struct ComponentB
{
    unsigned int data = 0;
};

template<typename... Args>
struct TypeList
{
};

template<typename T1, typename... Ts>
struct HasType
{
    static constexpr auto Value = (std::is_same_v<T1, Ts> || ...);
};

template<typename... ComponentTypes>
struct Components
{
    template<typename Component>
    struct Contains : public HasType<Component, ComponentTypes...>
    {
    };

    using Type = typename TypeList<ComponentTypes...>;
};

//template<typename Component, typename ...ComponentTypes>
//struct Components
//{
//    using Types = Components<ComponentTypes...>::Types;
//};

template<typename AccessList>
class View
{
private:
    template<typename... ViewComponents>
    constexpr static auto init(TypeList<ViewComponents...>, entt::registry& reg)
    {
        return reg.view<ViewComponents...>();
    }

    using ViewType =
        decltype(init(AccessList::Get::Type(), std::declval<entt::registry&>()) | init(AccessList::Update::Type(), std::declval<entt::registry&>()));

public:
    constexpr View(entt::registry& reg)
        : m_view(init(AccessList::Get::Type(), reg) | init(AccessList::Update::Type(), reg))
    {
    }

    constexpr auto begin() const
    {
        return m_view.begin();
    }

    constexpr auto end() const
    {
        return m_view.end();
    }

    template<typename Component>
    constexpr const Component& Get(entt::entity ent) const
    {
        static_assert(AccessList::Get::Contains<Component>::Value);
        return m_view.get<const Component>(ent);
    }

    template<typename Component>
    constexpr Component& Update(entt::entity ent) const
    {
        static_assert(AccessList::Update::Contains<Component>::Value);
        return m_view.get<Component>(ent);
    }

private:
    ViewType m_view;
};

class ISystem
{
public:
    virtual ~ISystem() = default;
};

class MySystem : public ISystem
{
public:
    struct AccessList
    {
        using Get = Components<ComponentA>;
        using Update = Components<ComponentB>;
    };

    void Update(const View<AccessList>& view)
    {
        for (auto ent : view)
        {
            spdlog::info("Component {} for entity {} = {}", typeid(ComponentA).name(), static_cast<be::uint32>(ent), view.Get<ComponentA>(ent).data);
            spdlog::info("Component {} for entity {} = {}", typeid(ComponentB).name(), static_cast<be::uint32>(ent), ++view.Update<ComponentB>(ent).data);
            //std::cout << comp.data++ << "\n";
        }
        /*std::cout << view.Get<ComponentA>() << "\n";
        std::cout << view.Get<ComponentB>() << "\n";*/
    }
};

class SystemRegister
{
public:
    template<typename System>
    void Add()
    {
        m_systems[std::type_index(typeid(System))] = be::CreateUnique<System>();
    }

    template<typename System>
    System* Get()
    {
        auto& system = m_systems.at(std::type_index(typeid(System)));
        return static_cast<System*>(system.get());
    }

private:
    std::unordered_map<std::type_index, be::Unique<ISystem>> m_systems;
};

class Scheduler
{
public:
    Scheduler(SystemRegister* reg, entt::registry& entreg)
        : m_sysReg(reg), m_entreg(entreg)
    {
    }

    template<typename System>
    void Attach()
    {
        auto* instance = m_sysReg->Get<System>();
        auto task = [system = instance](be::uint32, void* data, auto, auto) {
            entt::registry* reg = reinterpret_cast<entt::registry*>(data);
            View<System::AccessList> view(*reg);
            system->Update(view);
        };
        m_scheduler.attach(std::move(task));
        //m_tasks.push_back(std::move(task));
    }

    void Update()
    {
        /*for (const auto& task : m_tasks)
        {
            task(m_entreg);
        }*/
        m_scheduler.update(1, reinterpret_cast<void*>(&m_entreg));
    }

    void Abort()
    {
        m_scheduler.abort(true);
    }

private:
    SystemRegister* m_sysReg;
    entt::registry& m_entreg;
    //std::vector<std::function<void(entt::registry&)>> m_tasks;
    entt::scheduler<be::uint32> m_scheduler;
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

        entt::registry reg;
        auto ent = reg.create();
        reg.emplace_or_replace<ComponentA>(ent);
        reg.emplace_or_replace<ComponentB>(ent);

        ent = reg.create();
        reg.emplace_or_replace<ComponentA>(ent);
        reg.emplace_or_replace<ComponentB>(ent);

        SystemRegister sysRegister;
        sysRegister.Add<MySystem>();

        Scheduler scheduler(&sysRegister, reg);
        scheduler.Attach<MySystem>();

        while (m_isRunning)
        {
            m_window->ProcessInput();
            if (m_keyboard->IsKeyPressed(be::KeyCode::Escape))
            {
                scheduler.Abort();
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

    return be::CreateUnique<BasicApplication>(std::move(config), windowDescriptor);
}
