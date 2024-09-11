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
        m_engine->PrintInfo();
        Run();
    }
} // namespace be
