#pragma once
#include "Logger.h"

namespace be
{
	class ConsoleLogger final : public Logger
    {
    public:
        static UniquePtr<Logger> Create(const std::string& name);
        ConsoleLogger(LoggerImpl logger);
    };

    class FileLogger final : public Logger
    {
    public:
        static UniquePtr<Logger> Create(const std::string& name, const std::string& filePath);
        FileLogger(LoggerImpl logger);
    };
} // namespace be
