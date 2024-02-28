#include "Beast/Loggers/LoggersFactories.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace be
{
    static const std::string LOGGER_FILE_NAME_PREFIX = "fl_";

    Shared<Logger> ConsoleLogger::Create(const std::string& name)
    {
        LoggerImpl logger = spdlog::get(name);
        logger = (logger == nullptr ? spdlog::stdout_color_mt(name) : logger);

        return MakeShared<Logger>(logger);
    }

    Shared<Logger> FileLogger::Create(const std::string& name, const FilesystemPath& filePath)
    {
        const auto loggerName = LOGGER_FILE_NAME_PREFIX + name;
        LoggerImpl logger = spdlog::get(loggerName);
        logger = (logger == nullptr ? spdlog::basic_logger_mt(loggerName, filePath.string()) : logger);

        return MakeShared<Logger>(std::move(logger));
    }

    void FileLogger::Destroy(const std::string& name)
    {
        spdlog::drop(LOGGER_FILE_NAME_PREFIX + name);
    }
} // namespace be
