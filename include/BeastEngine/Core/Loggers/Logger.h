#pragma once
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Helpers.h"

#include <string> // Delete
#include <spdlog/logger.h>

namespace be
{
    using LoggerImpl = be::SharedPtr<spdlog::logger>;
    class Logger
    {
    public:
        BE_IMPLEMENT_CONSTRUCTORS_DEFAULT(Logger);
        virtual ~Logger() = default;

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

    protected:
        Logger(LoggerImpl logger);

    private:
        LoggerImpl m_logger = nullptr;
    };
} // namespace be
