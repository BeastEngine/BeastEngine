#pragma once
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Helpers.h"
#include "BeastEngine/Core/Loggers/Logger.h"

namespace be::internals
{
    using LoggerPtr = SharedPtr<Logger>;
    class StaticLogger final
    {
        friend class BeastEngine;
    public:
        BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(StaticLogger);

        template<typename... Args>
        static void LogInfo(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                Get()->LogInfo(message, args...);
            }
            catch (const std::exception&)
            {
            }
        }

        template<typename... Args>
        static void LogWarning(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                Get()->LogWarning(message, args...);
            }
            catch (const std::exception&)
            {
            }
        }

        template<typename... Args>
        static void LogError(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                Get()->LogError(message, args...);
            }
            catch (const std::exception&)
            {
            }
        }

        template<typename... Args>
        static void LogFatalError(const std::string& message, const Args&... args) noexcept
        {
            try
            {
                Get()->LogFatalError(message, args...);
            }
            catch (const std::exception&)
            {
            }
        }

    private:
        static void SetLogger(LoggerPtr logger)
        {
            StaticLogger::m_logger = std::move(logger);
        }

        static LoggerPtr Get()
        {
            if (m_logger == nullptr)
            {
                throw std::runtime_error("");
            }

            return m_logger;
        }

    private:
        inline static LoggerPtr m_logger = nullptr;
    };
} // namespace be
