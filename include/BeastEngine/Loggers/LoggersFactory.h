#pragma once
#include "LoggersTypes.h"
#include "BeastEngine/Definitions/Types.h"

namespace be::internals
{
    class LoggersFactory final
    {
    public:
        static ILogger Create(LoggerType loggerType);

    private:
        inline static const char* LOGGER_NAME_CONSOLE = "console_logger";
    };
}
