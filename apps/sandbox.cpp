#include <BeastEngine/EntryPoint.h>

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig, be::WindowHandleInstance wHInstance)
        : be::AApplication(std::move(engineConfig)), m_windowDescriptor(std::move(wHInstance))
    {
        m_window = m_engine->CreateMainWindow(m_windowDescriptor);
    }

    virtual void Run() override
    {
        m_engine->PrintInfo();
        while (true)
        {
            m_window->ProcessInput();
        }
    }

private:
    be::WindowDescriptor m_windowDescriptor;
    be::UniquePtr<be::IWindow> m_window;
};

be::UniquePtr<be::AApplication> be::CreateApplication(WindowHandleInstance windowHandleInstance)
{
    return be::CreateUniquePtr<BasicApplication>(be::EngineConfig(), std::move(windowHandleInstance));
}
