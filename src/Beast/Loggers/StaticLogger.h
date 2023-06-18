#pragma once
#include "Beast/Loggers/Logger.h"

#include <Beast/Common/Types.h>
#include <Beast/Common/Helpers.h>

namespace be::internals
{
    using LoggerPtr = Shared<Logger>;
    class StaticLogger final
    {
        friend class BeastEngine;

    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(StaticLogger);

        template<typename... Args>
        static constexpr void LogInfo(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogInfo(format, std::forward<Args>(args)...);
            }
        }

        template<typename... Args>
        static constexpr void LogWarning(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogWarning(format, std::forward<Args>(args)...);
            }
        }

        template<typename... Args>
        static void constexpr LogError(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogError(format, std::forward<Args>(args)...);
            }
        }

        template<typename... Args>
        static void constexpr LogFatalError(fmt::format_string<Args...> format, Args&&... args) noexcept
        {
            if (IsSet())
            {
                m_logger->LogFatalError(format, std::forward<Args>(args)...);
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
} // namespace be::internals
