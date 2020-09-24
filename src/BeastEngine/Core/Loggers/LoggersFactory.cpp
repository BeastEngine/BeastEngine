#include "BeastEngine/Core/Loggers/LoggersFactory.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace be::internals
{
    ILogger LoggersFactory::Create(LoggerType loggerType, const std::string& params)
    {
        ILogger logger = nullptr;
        switch (loggerType)
        {
        case LoggerType::LOGGER_CONSOLE:
            logger = spdlog::get(LOGGER_NAME_CONSOLE);
            return logger == nullptr ? spdlog::stdout_color_mt(LOGGER_NAME_CONSOLE) : logger;
        case LoggerType::LOGGER_FILE:
            logger = spdlog::get(LOGGER_NAME_FILE);
            return logger == nullptr ? spdlog::basic_logger_mt(LOGGER_NAME_FILE, params) : logger;
        default:
            return nullptr;
        }
    }
} // namespace be
