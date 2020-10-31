#pragma once
#include "BeastEngine/Core/Logging.h"
#include "BeastEngine/Core/Loggers/StaticLogger.h"

namespace be::internals
{
#ifndef BE_DEBUG_LOG_INFO
    #ifdef BE_DEBUGGING_INFO_ENABLED
        #define BE_DEBUG_LOG_INFO(message, ...) \
            internals::StaticLogger::LogInfo(BE_DEBUG_MESSAGE(message), __VA_ARGS__);
    #else
        #define BE_DEBUG_LOG_INFO(message, ...)
    #endif
#endif

#ifndef BE_DEBUG_LOG_WARNING
    #ifdef BE_DEBUGGING_INFO_ENABLED
        #define BE_DEBUG_LOG_WARNING(message, ...) \
            internals::StaticLogger::LogWarning(BE_DEBUG_MESSAGE(message), __VA_ARGS__)
    #else
        #define BE_DEBUG_LOG_WARNING(message, ...)
    #endif
#endif

#ifndef BE_DEBUG_LOG_ERROR
    #ifdef BE_DEBUGGING_INFO_ENABLED
        #define BE_DEBUG_LOG_ERROR(message, ...) \
            internals::StaticLogger::LogError(BE_DEBUG_MESSAGE(message), __VA_ARGS__)
    #else
        #define BE_DEBUG_LOG_ERROR(message, ...)
    #endif
#endif

#ifndef BE_DEBUG_LOG_FATAL_ERROR
    #ifdef BE_DEBUGGING_INFO_ENABLED
        #define BE_DEBUG_LOG_FATAL_ERROR(message, ...) \
            internals::StaticLogger::LogFatalError(BE_DEBUG_MESSAGE(message), __VA_ARGS__)
    #else
        #define BE_DEBUG_LOG_FATAL_ERROR(message, ...)
    #endif
#endif
} // namespace be::internals
