#include "BeastEngine/Core/Loggers/LoggersFactories.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace be
{
    static const std::string LOGGER_CONSOLE_NAME_PREFIX = "cs_";
    static const std::string LOGGER_FILE_NAME_PREFIX = "fl_";

    UniquePtr<Logger> ConsoleLogger::Create(const std::string& name)
    {
        const auto loggerName = LOGGER_CONSOLE_NAME_PREFIX + name;
        LoggerImpl logger = spdlog::get(loggerName);
        logger = (logger == nullptr ? spdlog::stdout_color_mt(loggerName) : logger);
        
        return CreateUniquePtr<ConsoleLogger>(logger);
    }
    
    ConsoleLogger::ConsoleLogger(LoggerImpl logger)
        : Logger(logger)
    {
    }

    UniquePtr<Logger> FileLogger::Create(const std::string& name, const std::string& filePath)
    {
        const auto loggerName = LOGGER_FILE_NAME_PREFIX + name;
        LoggerImpl logger = spdlog::get(loggerName);
        logger = (logger == nullptr ? spdlog::basic_logger_mt(loggerName, filePath) : logger);

        return CreateUniquePtr<FileLogger>(logger);
    }

    FileLogger::FileLogger(LoggerImpl logger)
        : Logger(logger)
    {
    }
} // namespace be
