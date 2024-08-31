#pragma once
#include "Beast/Common/Types.hpp"
#include "Beast/Common/Helpers.hpp"

#include <spdlog/logger.h>
#include <fmt/format.h>

namespace be
{
    using LoggerImpl = be::Shared<spdlog::logger>;

    /**
     * @brief Defines common interface for logging API, leaving the actual logging
     * functionality to the logger implementation provided during construction.
     * It accepts messages in the same format as the `fmt` and `spdlog` libraries, meaning you can use the same messages format.
     * @see https://fmt.dev/latest/index.hpptml
     */
    class Logger final
    {
    private:
#ifndef PRIV_LOG
    #define PRIV_LOG(func)                                       \
        try                                                      \
        {                                                        \
            m_logger->func(format, std::forward<Args>(args)...); \
            m_logger->flush();                                   \
        }                                                        \
        catch (const std::exception&)                            \
        {                                                        \
        }
#endif // !PRIV_LOG

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
        constexpr void LogInfo(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            PRIV_LOG(info);
        }

        /**
         * @breif Uses its logger implementation to log given message as a warning log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
         * @return
         */
        template<typename... Args>
        constexpr void LogWarning(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            PRIV_LOG(warn);
        }

        /**
         * @brief Uses its logger implementation to log given message as an error log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
         * @return 
         */
        template<typename... Args>
        constexpr void LogError(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            PRIV_LOG(error);
        }

        /**
         * @brief Uses its logger implementation to log given message as fatal error log.
         * 
         * @param message - Message to log
         * @param ...args - Arguments to be parsed into the given message
         * @return 
         */
        template<typename... Args>
        constexpr void LogFatalError(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            PRIV_LOG(critical);
        }

    private:
        LoggerImpl m_logger = nullptr;
    };
} // namespace be
