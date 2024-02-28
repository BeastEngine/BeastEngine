#include <Beast/EntryPoint.h>
#include <Beast/BeastEngine.h>
#include <Beast/Loggers/LoggersFactories.h>
#include <Beast/Common/Types.h>
#include <Beast/Common/Utils/Hasher.h>

#include <Beast/Ecs/Types.h>
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/SystemsScheduler.h>
#include <Beast/Ecs/Components/Graphics.h>

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

constexpr be::Id TEXTURE_ID = be::Id("Path/To/My/Texture");

class Attacher
{
public:
    struct AccessList : be::BaseAccessList
    {
        using Add = be::Components<be::Sprite, be::Transform>;
    };

    void Run(const be::View<AccessList>& view)
    {
        be::uint32 layer = 0;
        for (std::size_t i = 0; i < 10; ++i)
        {
            const auto entity = view.CreateEntity();
            view.AddComponent(
                entity,
                be::Sprite{
                    .texture{.id = TEXTURE_ID, .uvCoords = {0.0f, 1.0f}},
                    .material{.color{}},
                    .layer = be::ToEnum<be::Layer>(layer),
                }
            );

            layer = (layer + 1) % be::LAYERS_COUNT;
        }
    }
};

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig, const be::WindowDescriptor& windowDescriptor)
        : be::AApplication(std::move(engineConfig), windowDescriptor), m_logger(be::ConsoleLogger::Create("client_console_logger"))
    {
        m_window->SetWindowClosedEventHandler(OnWindowClosedCustom());
        m_mouse->SetWheelScrolledListener(OnWheelScrolled());

        GetEngine().PrintInfo();
    }

    void Run() override
    {
        GetEngine().PrintInfo();

        auto& scheduler = m_ecs.scheduler;

        auto group2 = scheduler.CreateGroup();
        group2.AttachSystem<Attacher>();

        /*be::Shared<be::graphics::Graphics> graphics = GetEngine().CreateGraphics(be::graphics::RenderingApi::D3D11, *m_window);
        group2.AttachSystem<Renderer>(graphics);*/

        auto group1 = scheduler.CreateGroup();

        scheduler.Prepare({group2, group1});

        be::Vec2i previousCords = m_mouse->GetMousePosition();
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
    be::WindowClosedEventHandler OnWindowClosedCustom()
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

be::Unique<be::AApplication> be::CreateApplication(WindowHandleInstanceType windowHandleInstance)
{
    // Configure engine
    auto config = be::EngineConfig();

    // Configure window
    be::WindowDescriptor windowDescriptor(std::move(windowHandleInstance));
    windowDescriptor.style = WindowStyle::WINDOW_DEFUALT;

    return be::MakeUnique<BasicApplication>(std::move(config), windowDescriptor);
}
