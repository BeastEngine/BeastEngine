#include "BeastEngine/Core/BeastEngine.h"
#include "BeastEngine/Core/Logging.h"
#include "BeastEngine/Core/Assertions.h"
#include "BeastEngine/Core/Versions.h"
#include "BeastEngine/Core/Windows/WindowFactory.h"

#include <fmt/color.h>

namespace be
{
    BeastEngine::BeastEngine(EngineConfig config)
        : m_config(std::move(config))
    {
        SetLogger();
        SetWindowFactory();
    }

    void BeastEngine::PrintInfo() const
    {
        const auto engineNameString = fmt::format(fmt::emphasis::bold, "BeastEngine - ");
        const auto versionString = fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold, BEAST_ENGINE_VERSION);
        fmt::print("{}Version [{}]", engineNameString, versionString);
    }

    UniquePtr<IWindow> BeastEngine::CreateMainWindow(const WindowDescriptor& descriptor) const
    {
        return m_config.windowFactory->Create(descriptor);
    }

    void BeastEngine::SetLogger() const
    {
        internals::StaticLogger::SetLogger(
            std::move(
                internals::LoggersFactory::Create(m_config.staticLogger.type, m_config.staticLogger.additionalParams)
            )
        );
    }
    
    void BeastEngine::SetWindowFactory()
    {
        if (m_config.windowFactory == nullptr)
        {
            m_config.windowFactory = CreateUniquePtr<internals::WindowFactory>();
        }
    }
} // namespace be
