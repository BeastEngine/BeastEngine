#include <Beast/Core/EntryPoint.h>
#include <Beast/Core/BeastEngine.h>
#include <Beast/Core/Loggers/LoggersFactories.h>

#include <iostream>

namespace render
{
    class IVertexShader;
    class IPixelShader;
    class IBuffer;

    struct RenderPass
    {
        IVertexShader* vertexShader;
        IPixelShader* pixelShader;

        std::vector<IBuffer*> buffers;
    };

    struct Pipeline
    {
        std::vector<RenderPass> passes;
    };

    enum class BufferType
    {
        BE_VERTEX_BUFFER,
        BE_INDEX_BUFFER,
    };
    
    struct BufferDescriptor
    {
        BufferType type;
    };

    class IContext
    {
        virtual be::Unique<IVertexShader> CreateVertexShader() = 0;
        virtual be::Unique<IBuffer> CreateBuffer(const BufferDescriptor& descriptor) = 0;

        virtual void Render(const Pipeline& pipeline) = 0;
    };

    class IDevice
    {
        virtual be::Unique<IContext> CreateContex() = 0;
    };
}

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
