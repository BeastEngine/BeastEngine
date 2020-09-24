#include <BeastEngine/EntryPoint.h>
#include <BeastEngine/Core/Loggers/StaticLogger.h>

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig, be::WindowHandleInstance wHInstance)
        : be::AApplication(std::move(engineConfig)), m_windowDescriptor(std::move(wHInstance))
    {
        m_windowDescriptor.style = be::WindowStyle::WINDOW_DEFUALT;

        m_window = m_engine->CreateMainWindow(m_windowDescriptor);
        m_window->SetWindowClosedEventHandler([&]() {
            m_isRunning = false;
        });
    }

    virtual void Run() override
    {
        m_engine->PrintInfo();

        while (m_isRunning)
        {
            m_window->ProcessInput();
        }
    }

private:
    bool m_isRunning = true;

    be::WindowDescriptor m_windowDescriptor;
    be::UniquePtr<be::IWindow> m_window;
};

be::UniquePtr<be::AApplication> be::CreateApplication(WindowHandleInstance windowHandleInstance)
{
    auto config = be::EngineConfig();
    config.staticLogger.type = be::LoggerType::LOGGER_CONSOLE;

    return be::CreateUniquePtr<BasicApplication>(std::move(config), std::move(windowHandleInstance));
}
