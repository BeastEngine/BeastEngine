#pragma once
#include "Beast/Loggers/Logger.h"

#include <Beast/Common/Helpers.h>
#include <Beast/Common/Filesystem/Types.h>

namespace be
{
    /**
     * @brief Static factory providing console loggers.
     */
    class ConsoleLogger final
    {
    public:
        CT_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ConsoleLogger);

        /**
         * @brief Creates Logger instance with appropriate console logger implementation.
         * Passed name is used to identify logger implementation.
         * If logger implementation with given name already exists, new Logger object with that implementation is returned.
         * 
         * @param name - Logger implementation string id
         * @return
         */
        static Shared<Logger> Create(const std::string& name);
    };

    /**
     * @brief Static factory providing file loggers.
     */
    class FileLogger final
    {
    public:
        CT_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(FileLogger);

        /**
         * @brief Creates Logger instance with appropriate file logger implementation.
         * Passed name is used to identify logger implementation.
         * Created Logger will log messages into the file represented by the given file path.
         * If logger implementation with given name already exists, new Logger object with that implementation is returned.
         * If logger implementation already exists, filePath is ignored, and returned logger operates on the filePath provided during the first creation of the logger with given name.
         * If you want to create logger with the same name, but different filePath, you have to call Destroy() first.
         * @code
         * {
         *      // Creating the `foo` logger for the first time
         *      auto logger1 = FileLogger::Create("foo", "foo_file.log");
         * 
         *      // Same name but different filePath. filePath ignored, logger2 still uses "foo_file.log";
         *      auto logger2 = FileLogger::Create("foo", "foo_file2.log");
         * }
         * @endcode
         * @code
         * {
         *      // Creating the `foo` logger for the first time
         *      auto logger1 = FileLogger::Create("foo", "foo_file.log");
         * 
         *      FileLogger::Destroy("foo"); // Foo logger destroyed. "foo_file.log" no longer in usage
         *      
         *      // Same name but different filePath. New `foo` logger created using "foo_file2.log"
         *      auto logger2 = FileLogger::Create("foo", "foo_file2.log");
         * }
         * @endcode
         * 
         * @see Destroy()
         * 
         * @param name - Logger implementation string id
         * @param filePath - Path to a file where all the logs will be stored
         * @return
         */
        static Shared<Logger> Create(const std::string& name, const FilesystemPath& filePath);

        /**
         * @brief Destroys logger implementation associated with given name.
         * This means that after calling this method, you can create a new logger with this name, but different filePath for example.
         * File logger contain handles to their files, meaning that in order to delete such file, logger implementation must release the handle.
         * You can use this method to destroy implementation from the registry, thus, force it to release the handle.
         * NOTE1: The handle will be released only once the last instance of the implementation is destroyed.
         * NOTE2: This method removes only the logger implementation, not any Logger class instance.
         * This means that if any Logger contains reference to the logger implementation associated with given name, it will still be valid after call to this method.
         * This means that there might be a case where two separate logger implementations use the same logging file.
         * Generally, this is not a problem, but keep that in mind!
         * @code
         * {
         *      auto logger1 = FileLogger::Create("foo", "foo_file.log");
         *      FileLogger::Destroy("foo"); // Removes logger implementation from the registry, but one instance still exists inside logger1!
         * 
         *      // Creates new logger implementation of name "foo", using "foo_file.log";
         *      // Two different loggers are now using the "foo_file.log" file!
         *      auto logger2 = FileLogger::Create("foo", "foo_file.log");
         * }
         * @endcode
         * 
         * @see Create()
         * 
         * @param name - Name of the logger previously created via Create() method
         */
        static void Destroy(const std::string& name);
    };
} // namespace be
