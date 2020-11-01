#pragma once
#include "Logger.h"
#include "BeastEngine/Core/Helpers.h"

namespace be
{
    class ConsoleLogger final
    {
    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ConsoleLogger);

        /**
         * Creates Logger instance with appropriate console logger implementation.
         *  Passed name is used to identify logger implementation.
         *  If logger implementation with given name already exists, new Logger object with that implementation is returned.
         * 
         * @param name - logger implementation string id
         * @return Unique pointer to Logger instance
         */
        static UniquePtr<Logger> Create(const std::string& name);
    };

    class FileLogger final
    {
    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(FileLogger);

        /**
         * Creates Logger instance with appropriate file logger implementation.
         *  Passed name is used to identify logger implementation.
         *  Created Logger will log messages into the file represented by the given file path.
         *  If logger implementation with given name already exists, new Logger object with that implementation is returned.
         * 
         * @param name - logger implementation string id
         * @param filePath - path to a file, which all the logs will be stored in
         * @return Unique pointer to Logger instance
         */
        static UniquePtr<Logger> Create(const std::string& name, const std::string& filePath);
    };
} // namespace be
