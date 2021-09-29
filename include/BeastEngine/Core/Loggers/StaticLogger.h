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
            // TODO: Try to get rid of the try-catch
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
                throw std::exception();
            }

            return m_logger;
        }

    private:
        inline static LoggerPtr m_logger = nullptr;
    };
} // namespace be
