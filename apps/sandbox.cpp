#include <BeastEngine/EntryPoint.h>
#include <BeastEngine/Core/Logging.h>

#include <iostream>

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
        m_engine->PrintInfo();

        while (m_isRunning)
        {
            const auto& coords = m_mouse->GetCoordinates();

            m_window->ProcessInput();
            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_LEFT))
            {
                m_logger->LogInfo("Left  button pressed");
                m_logger->LogInfo("Mouse coords: [" + std::to_string(coords.x) + ", " + std::to_string(coords.y) + "]\n");
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_MIDDLE))
            {
                m_logger->LogInfo("Middle button pressed");
                m_logger->LogInfo("Mouse coords: [" + std::to_string(coords.x) + ", " + std::to_string(coords.y) + "]\n");
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_RIGHT))
            {
                m_logger->LogInfo("Right button pressed");
                m_logger->LogInfo("Mouse coords: [" + std::to_string(coords.x) + ", " + std::to_string(coords.y) + "]\n");
            }

            if (m_mouse->IsButtonHeldDown(be::MouseButtonCode::BUTTON_LEFT))
            {
                m_logger->LogInfo("Left button held down");
                m_logger->LogInfo("Mouse coords: [" + std::to_string(coords.x) + ", " + std::to_string(coords.y) + "]\n");
            }

            if (m_mouse->IsButtonHeldDown(be::MouseButtonCode::BUTTON_MIDDLE))
            {
                m_logger->LogInfo("Middle button held down");
                m_logger->LogInfo("Mouse coords: [" + std::to_string(coords.x) + ", " + std::to_string(coords.y) + "]\n");
            }

            if (m_mouse->IsButtonHeldDown(be::MouseButtonCode::BUTTON_RIGHT))
            {
                m_logger->LogInfo("Right button held down");
                m_logger->LogInfo("Mouse coords: [" + std::to_string(coords.x) + ", " + std::to_string(coords.y) + "]\n");
            }

            if (m_keyboard->IsKeyPressed(be::KeyCode::LeftShift))
            {
                m_logger->LogInfo("Right arrow pressed\n");
            }

            if (m_keyboard->IsKeyHeldDown(be::KeyCode::LeftShift))
            {
                m_logger->LogInfo("Right arrow held down!\n");
            }

            if (m_keyboard->IsKeyDown(be::KeyCode::LeftShift))
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
    const be::UniquePtr<be::Logger> m_logger = nullptr;
};

be::UniquePtr<be::AApplication> be::CreateApplication(WindowHandleInstance windowHandleInstance)
{
    // Configure engine
    auto config = be::EngineConfig();

    // Configure window
    auto windowDescriptor = be::WindowDescriptor(std::move(windowHandleInstance));
    windowDescriptor.style = WindowStyle::WINDOW_DEFUALT;

    return be::CreateUniquePtr<BasicApplication>(std::move(config), windowDescriptor);
}
