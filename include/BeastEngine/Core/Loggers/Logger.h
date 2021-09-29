#pragma once
#include <Common/Types.h>
#include <Common/Helpers.h>

#include <spdlog/logger.h>

namespace be
{
    using LoggerImpl = be::Shared<spdlog::logger>;

    /**
     * @brief Defines common interface for logging API, leaving the actual logging
     * functionality to the logger implementation provided during construction.
     * It accepts messages in the same format as the `fmt` and `spdlog` libraries, meaning you can use the same messages format.
     * @see https://fmt.dev/latest/index.html
     */
    class Logger final
    {
    public:
        /**
         * @brief Creates logger object which will use passed LoggerImpl for actual logging.
         * If passed implementation is nullptr, throws exception.
         * 
         * @param logger - Implementation to use for logging
         */
        Logger(LoggerImpl logger);

        /**
         * @brief Uses its logger implementation to log given message as an informational log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
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
         * @breif Uses its logger implementation to log given message as a warning log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
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
         * @brief Uses its logger implementation to log given message as an error log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
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
         * @brief Uses its logger implementation to log given message as fatal error log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
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
