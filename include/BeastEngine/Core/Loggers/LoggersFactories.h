#pragma once
#include "Logger.h"
#include "BeastEngine/Core/Helpers.h"

namespace be
{
	class ConsoleLogger final
    {
    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ConsoleLogger);
        static UniquePtr<Logger> Create(const std::string& name);
    };

    class FileLogger final
    {
    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(FileLogger);
        static UniquePtr<Logger> Create(const std::string& name, const std::string& filePath);
    };
} // namespace be
