#include "Beast/Core/Loggers/LoggersFactories.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace be
{
    static const std::string LOGGER_CONSOLE_NAME_PREFIX = "cs_";
    static const std::string LOGGER_FILE_NAME_PREFIX = "fl_";

    Shared<Logger> ConsoleLogger::Create(const std::string& name)
    {
        const auto loggerName = LOGGER_CONSOLE_NAME_PREFIX + name;
        LoggerImpl logger = spdlog::get(loggerName);
        logger = (logger == nullptr ? spdlog::stdout_color_mt(loggerName) : logger);
        
        return CreateShared<Logger>(logger);
    }

    Shared<Logger> FileLogger::Create(const std::string& name, const FilesystemPath& filePath)
    {
        const auto loggerName = LOGGER_FILE_NAME_PREFIX + name;
        LoggerImpl logger = spdlog::get(loggerName);
        logger = (logger == nullptr ? spdlog::basic_logger_mt(loggerName, filePath.string()) : logger);

        return CreateShared<Logger>(std::move(logger));
    }
    
    void FileLogger::Destroy(const std::string& name)
    {
        spdlog::drop(LOGGER_FILE_NAME_PREFIX + name);
    }
} // namespace be
