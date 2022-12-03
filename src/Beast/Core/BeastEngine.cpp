#include "Beast/Core/BeastEngine.h"
#include "Beast/Core/Versions.h"
#include "Beast/Core/Windows/WindowFactory.h"
#include "Beast/Core/Loggers/LoggersFactories.h"
#include "Beast/Core/Loggers/StaticLogger.h"

#include <fmt/color.h>

namespace be
{
    static constexpr const char* DEFAULT_LOGGER_NAME = "beast_engine_default_logger";

    BeastEngine::BeastEngine(EngineConfig config)
    {
        SetLogger(config);
        SetWindowFactory(config);
    }

    void BeastEngine::PrintInfo() const
    {
        const auto engineNameString = fmt::format(fmt::emphasis::bold, "BeastEngine - ");
        const auto versionString = fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold, BEAST_ENGINE_VERSION);
        fmt::print("{}Version [{}]\n", engineNameString, versionString);
    }

    Unique<IWindow> BeastEngine::CreateNewWindow(const WindowDescriptor& descriptor) const
    {
        return m_windowFactory->Create(descriptor);
    }

    void BeastEngine::SetLogger(EngineConfig& config)
    {
        internals::LoggerPtr logger = std::move(config.logger);
        if (logger == nullptr)
        {
            logger = ConsoleLogger::Create(DEFAULT_LOGGER_NAME);
        }

        internals::StaticLogger::SetLogger(std::move(logger));
    }

    void BeastEngine::SetWindowFactory(EngineConfig& config)
    {
        m_windowFactory = std::move(config.windowFactory);
        if (m_windowFactory == nullptr)
        {
            m_windowFactory = CreateUnique<internals::WindowFactory>();
        }
    }
} // namespace be
