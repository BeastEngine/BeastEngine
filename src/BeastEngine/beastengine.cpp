#include "BeastEngine/BeastEngine.h"
#include "BeastEngine/Loggers/StaticLogger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace be
{
    BeastEngine::BeastEngine(EngineConfig config)
        : m_config(std::move(config))
    {
        SetLogger(config);

        internals::StaticLogger::LogInfo("[TEST] Engine Initialized with {}", "No errors");
        internals::StaticLogger::LogWarning("[TEST] Engine Initialized with {}", "Warning");
        internals::StaticLogger::LogError("[TEST] Engine Initialized with {}", "Error");
        internals::StaticLogger::LogFatalError("[TEST] Engine Initialized with {}", "Fatal Error");
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
