#include "Beast/Application.h"

namespace be
{
    AApplication::AApplication(EngineConfig engineConfig, const WindowDescriptor& mainWindowDescriptor)
    {
        m_engine = MakeUnique<BeastEngine>(std::move(engineConfig));
        m_window = m_engine->CreateNewWindow(mainWindowDescriptor);
    }

    void AApplication::Start()
    {
        try
        {
            m_engine->PrintInfo();
            Run();
        }
        catch (const std::exception& error)
        {
            BE_DISPLAY_ERROR(error);
        }
    }
} // namespace be
