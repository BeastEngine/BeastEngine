#pragma once
#include "Beast/Loggers/StaticLogger.h"

#include <fmt/format.h>

namespace be::internals
{
#ifndef BE_DEBUG_MESSAGE
    #define BE_DEBUG_MESSAGE(message) "[{}:{}] *** " message " ***", __FILE__, __LINE__
#endif

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
            internals::StaticLogger::LogError(message, __VA_ARGS__)
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

#ifdef BE_DEBUG
    #define BE_DEBUG_FIELD(field) field
    #define BE_DEBUG_EXPRESSION(expression) expression
#else
    #define BE_DEBUG_FIELD(field)
    #define BE_DEBUG_EXPRESSION(expression)
#endif
} // namespace be::internals
