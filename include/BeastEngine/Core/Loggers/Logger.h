#pragma once
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Helpers.h"

#include <spdlog/logger.h>

namespace be
{
    using LoggerImpl = be::SharedPtr<spdlog::logger>;
    class Logger
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT(Logger);

        /**
         * Creates logger object which will use passed LoggerImpl for actual logging.
         *  If passed implementation is nullptr, throws exception
         * 
         * @param logger - implementation to use for actual logging
         */
        Logger(LoggerImpl logger);
        virtual ~Logger() = default;

        /**
         * Uses its logger implementation to log given message as informational log.
         * 
         * @param message - message to log
         * @param ...args - arguments to be parsed into the given message if it contains any placeholders ('{}')
         * @return 
         */
        template<typename... Args>
        void LogInfo(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                m_logger->info(message, args...);
                m_logger->flush();
            }
            catch (const std::exception&)
            {
            }
        }

        /**
         * Uses its logger implementation to log given message as warning log.
         * 
         * @param message - message to log
         * @param ...args - arguments to be parsed into the given message if it contains any placeholders ('{}')
         * @return 
         */
        template<typename... Args>
        void LogWarning(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                m_logger->warn(message, args...);
                m_logger->flush();
            }
            catch (const std::exception&)
            {
            }
        }

        /**
         * Uses its logger implementation to log given message as error log.
         * 
         * @param message - message to log
         * @param ...args - arguments to be parsed into the given message if it contains any placeholders ('{}')
         * @return 
         */
        template<typename... Args>
        void LogError(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                m_logger->error(message, args...);
                m_logger->flush();
            }
            catch (const std::exception&)
            {
            }
        }

        /**
         * Uses its logger implementation to log given message as fatal error log.
         * 
         * @param message - message to log
         * @param ...args - arguments to be parsed into the given message if it contains any placeholders ('{}')
         * @return 
         */
        template<typename... Args>
        void LogFatalError(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                m_logger->critical(message, args...);
                m_logger->flush();
            }
            catch (const std::exception&)
            {
            }
        }

    private:
        LoggerImpl m_logger = nullptr;
    };
} // namespace be
