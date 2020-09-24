#pragma once
#include "LoggersTypes.h"
#include "BeastEngine/Core/Types.h"

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
         * @param params - Additional parameters for the logger to use (eg. filename for file logger)
         * @return ILogger implementation
         */
        static ILogger Create(LoggerType loggerType, const std::string& params);

    private:
        inline static const char* LOGGER_NAME_CONSOLE = "console_logger";
        inline static const char* LOGGER_NAME_FILE = "file_logger";
    };
}
