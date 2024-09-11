#include "Beast/BeastEngine.h"
#include "Beast/Versions.h"

#include "Beast/Loggers/LoggersFactories.h"
#include "Beast/Loggers/StaticLogger.h"

#include "Beast/Graphics/GraphicsFactory.h"

#include <fmt/color.h>

namespace be
{
    static constexpr const char* DEFAULT_LOGGER_NAME = "be-core";

    BeastEngine::BeastEngine(EngineConfig config)
    {
        SetLogger(config);
    }

    void BeastEngine::PrintInfo() const
    {
        const auto engineNameString = fmt::format(fmt::emphasis::bold, "BeastEngine - ");
        const auto versionString = fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold, BEAST_ENGINE_VERSION);
        fmt::print("{}Version [{}]\n", engineNameString, versionString);
    }

    Unique<Window> BeastEngine::CreateNewWindow(const WindowDescriptor& descriptor) const
    {
        return MakeUnique<Window>(descriptor);
    }

    Unique<graphics::IGraphics> BeastEngine::CreateGraphics(const Window& window, graphics::RenderingApi api) const
    {
        return internals::CreateGraphics(api, window);
    }

    void BeastEngine::SetLogger(EngineConfig& config)
    {
        LoggerPtr logger = std::move(config.logger);
        if (logger == nullptr)
        {
            logger = ConsoleLogger::Create(DEFAULT_LOGGER_NAME);
        }

        StaticLogger::SetLogger(std::move(logger));
    }
} // namespace be
