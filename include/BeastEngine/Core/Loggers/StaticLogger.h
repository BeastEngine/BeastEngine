#pragma once
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Helpers.h"

namespace be::internals
{
    class StaticLogger final
    {
        friend class BeastEngine;

    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(StaticLogger)

        template<typename... Args>
        static void LogInfo(const std::string& message, const Args&... args)
        {
            m_logger->info(message, args...);
        }

        template<typename... Args>
        static void LogWarning(const std::string& message, const Args&... args)
        {
            m_logger->warn(message, args...);
        }

        template<typename... Args>
        static void LogError(const std::string& message, const Args&... args)
        {
            m_logger->error(message, args...);
        }

        template<typename... Args>
        static void LogFatalError(const std::string& message, const Args&... args)
        {
            m_logger->critical(message, args...);
        }

    private:
        static void SetLogger(ILogger logger)
        {
            StaticLogger::m_logger = std::move(logger);
        }

    private:
        inline static ILogger m_logger = nullptr;
    };
} // namespace be::internals
