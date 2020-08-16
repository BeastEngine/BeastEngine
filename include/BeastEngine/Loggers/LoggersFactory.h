#pragma once
#include "LoggersTypes.h"
#include "BeastEngine/Definitions/Types.h"

namespace be::internals
{
    class LoggersFactory final
    {
    public:        
        /**
         * Returns logger implementation based on given type.
         * It should be used to create desired loggers.
         * 
         * Example: const auto logger = LoggersFactory::Create(LoggerType::LOGGER_CONSOLE);
         * 
         * @param loggerType - Logger to create
         * @return ILogger implementation
         */
        static ILogger Create(LoggerType loggerType);

    private:
        inline static const char* LOGGER_NAME_CONSOLE = "console_logger";
    };
}
