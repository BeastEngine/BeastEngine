#include "BeastEngine/Core/BeastEngine.h"
#include "BeastEngine/Core/Logging.h"
#include "BeastEngine/Core/Assertions.h"
#include "BeastEngine/Core/Versions.h"

#include <fmt/color.h>

namespace be
{
    BeastEngine::BeastEngine(EngineConfig config)
        : m_config(std::move(config))
    {
        SetLogger(config);
    }

    void BeastEngine::PrintInfo() const
    {
        const auto engineNameString = fmt::format(fmt::emphasis::bold, "BeastEngine - ");
        const auto versionString = fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold, BEAST_ENGINE_VERSION);
        fmt::print("{}Version [{}]", engineNameString, versionString);
    }

    void BeastEngine::SetLogger(const EngineConfig& config) const
    {
        internals::StaticLogger::SetLogger(
            std::move(
                internals::LoggersFactory::Create(config.staticLoggerType)
            )
        );
    }
} // namespace be
