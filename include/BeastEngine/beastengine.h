#pragma once
#include "BeastEngine/Definitions/Types.h"
#include "BeastEngine/Definitions/Helpers.h"
#include "BeastEngine/Loggers/LoggersFactory.h"
#include "BeastEngine/Loggers/LoggersTypes.h"

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
        
        /**
         * Initializes all engine subsystems using passed configuration.
         * 
         * @param config - Engine's subsystems configuration struct
         */
        BeastEngine(EngineConfig config);
        ~BeastEngine() = default;

        /**
         * Prints information about current engine's version into the stdout.
         */
        void PrintInfo() const;

    private:
        void SetLogger(const EngineConfig& config) const;

    private:
        const EngineConfig m_config;
    };
} // namespace be
