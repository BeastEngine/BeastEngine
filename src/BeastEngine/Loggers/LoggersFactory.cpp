#include "BeastEngine/Loggers/LoggersFactory.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace be::internals
{
    ILogger LoggersFactory::Create(LoggerType loggerType)
    {
        ILogger logger = nullptr;
        switch (loggerType)
        {
        case LoggerType::LOGGER_CONSOLE:
            logger = spdlog::get(LOGGER_NAME_CONSOLE);
            return logger == nullptr ? spdlog::stdout_color_mt(LOGGER_NAME_CONSOLE) : logger;
        default:
            return nullptr;
        }
    }
} // namespace be
