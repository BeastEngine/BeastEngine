#include <BeastEngine/EntryPoint.h>
#include <BeastEngine/Core/Loggers/StaticLogger.h>

#include <iostream>

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig, const be::WindowDescriptor& windowDescriptor)
        : be::AApplication(std::move(engineConfig), windowDescriptor)
    {
        m_window->SetWindowClosedEventHandler(OnWindowClosed());
        m_mouse->SetWheelScrolledListener(OnWheelScrolled());
    }

    virtual void Run() override
    {
        m_engine->PrintInfo();

        while (m_isRunning)
        {
            const auto& coords = m_mouse->GetCoordinates();

            m_window->ProcessInput();
            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_LEFT))
            {
                std::cout << "Left button pressed\n";
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_MIDDLE))
            {
                std::cout << "Middle button pressed\n";
            }

            if (m_mouse->IsButtonPressed(be::MouseButtonCode::BUTTON_RIGHT))
            {
                std::cout << "Right button pressed\n";
            }
            /*system("CLS");
            std::cout << "Mouse coords: [" << coords.x << ", " << coords.y << "]\n";*/
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
};

be::UniquePtr<be::AApplication> be::CreateApplication(WindowHandleInstance windowHandleInstance)
{
    // Configure engine
    auto config = be::EngineConfig();
    config.staticLogger.type = be::LoggerType::LOGGER_CONSOLE;

    // Configure window
    auto windowDescriptor = be::WindowDescriptor(std::move(windowHandleInstance));
    windowDescriptor.style = WindowStyle::WINDOW_DEFUALT;

    return be::CreateUniquePtr<BasicApplication>(std::move(config), windowDescriptor);
}
