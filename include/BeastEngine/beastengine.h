#pragma once
#include "BeastEngine/Definitions/Types.h"
#include "BeastEngine/Definitions/Helpers.h"
#include "BeastEngine/Loggers/LoggersFactory.h"
#include "BeastEngine/Loggers/LoggersTypes.h"

#include <string>

namespace be
{
    struct EngineConfig
    {
        LoggerType staticLoggerType = LoggerType::LOGGER_CONSOLE;
    };

    class BeastEngine final
    {
    public:
        IMPLEMENT_CONSTRUCTORS_DELETED(BeastEngine)

        BeastEngine(EngineConfig config);
        ~BeastEngine() = default;

    private:
        void SetLogger(const EngineConfig& config) const;

    private:
        const EngineConfig m_config;
    };
} // namespace be
