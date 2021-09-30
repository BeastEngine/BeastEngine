#pragma once
#include "BeastEngine/Core/Loggers/Logger.h"

#include <Common/Types.h>
#include <Common/Helpers.h>

namespace be::internals
{
    using LoggerPtr = Shared<Logger>;
    class StaticLogger final
    {
        friend class BeastEngine;
    public:
        CT_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(StaticLogger);

        template<typename... Args>
        static void LogInfo(const std::string& message, const Args&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogInfo(message, args...);
            }
        }

        template<typename... Args>
        static void LogWarning(const std::string& message, const Args&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogWarning(message, args...);
            }
        }

        template<typename... Args>
        static void LogError(const std::string& message, const Args&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogError(message, args...);
            }
        }

        template<typename... Args>
        static void LogFatalError(const std::string& message, const Args&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogFatalError(message, args...);
            }
        }

    private:
        static void SetLogger(LoggerPtr logger)
        {
            StaticLogger::m_logger = std::move(logger);
        }

        static bool IsSet()
        {
            return m_logger != nullptr;
        }

    private:
        inline static LoggerPtr m_logger = nullptr;
    };
} // namespace be
