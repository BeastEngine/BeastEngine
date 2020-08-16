#include "BeastEngine/BeastEngine.h"
#include "BeastEngine/Loggers/StaticLogger.h"
#include "BeastEngine/Logging.h"

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

        BE_DEBUG_LOG_INFO("[TEST] Engine Initialized with {}{}", "Macro", " with No errors");
        BE_DEBUG_LOG_WARNING("[TEST] Engine Initialized with {}{}", "Macro", " with Warning");
        BE_DEBUG_LOG_ERROR("[TEST] Engine Initialized with {}{}", "Macro", " with Error");
        BE_DEBUG_LOG_FATAL_ERROR("[TEST] Engine Initialized with {}{}", "Macro", " with Fatal Error");
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
