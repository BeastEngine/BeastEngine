#pragma once
#include "Beast/PlatformSetup.hpp"
#include "Beast/Loggers/StaticLogger.hpp"

#include <format>

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
    #define BE_DEBUG_FIELD(field)           field
    #define BE_DEBUG_EXPRESSION(expression) expression
#else
    #define BE_DEBUG_FIELD(field)
    #define BE_DEBUG_EXPRESSION(expression)
#endif

#ifndef BE_DEBUG_BREAK
    #ifdef _MSC_VER
        #define BE_DEBUG_BREAK() __debugbreak();
    #else
        #define BE_DEBUG_BREAK() static_assert(false, "Not supported!");
    #endif
#endif

#ifndef BE_CRITICAL_ASSERT
    #ifdef BE_ASSERTIONS_ENABLED
        #define BE_CRITICAL_ASSERT(expression)                                                                                \
            if (expression)                                                                                                   \
            { /* This is intentionally empty. Solitare 'if' could lead to potenial bugs */                                    \
            }                                                                                                                 \
            else                                                                                                              \
            {                                                                                                                 \
                BE_DEBUG_LOG_FATAL_ERROR("The critical '{}' assertion failed! It evaluated to: {}", #expression, expression); \
                BE_DEBUG_BREAK();                                                                                             \
            }
    #else
        #define BE_CRITICAL_ASSERT(expression)
    #endif
#endif

#define BE_ASSERT_PRIV_ALWAYS(expression, fmt, ...)                                \
    if (expression)                                                                \
    { /* This is intentionally empty. Solitare 'if' could lead to potenial bugs */ \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        BE_DEBUG_LOG_ERROR(fmt, __VA_ARGS__);                                      \
        BE_DEBUG_BREAK();                                                          \
    }

#ifdef BE_ASSERTIONS_ENABLED
    #define BE_ASSERT_PRIV(expression, fmt, ...) BE_ASSERT_PRIV_ALWAYS(expression, fmt, __VA_ARGS__)
#else
    #define BE_ASSERT_PRIV(expression, fmt, ...)
#endif

#define BE_ASSERT(expression)                      BE_ASSERT_PRIV(expression, "The '{}' assertion failed! It evaluated to: {}", #expression, expression)
#define BE_ASSERT_ALWAYS(expression)               BE_ASSERT_PRIV_ALWAYS(expression, "The '{}' assertion failed! It evaluated to: {}", #expression, expression)
#define BE_ASSERT_MSG(expression, fmt, ...)        BE_ASSERT_PRIV(expression, fmt, __VA_ARGS__)
#define BE_ASSERT_MSG_ALWAYS(expression, fmt, ...) BE_ASSERT_PRIV_ALWAYS(expression, fmt, __VA_ARGS__)

#ifndef BE_EXCEPTION_MESSAGE
    #ifdef BE_DEBUG
        #define BE_EXCEPTION_MESSAGE(message) std::format("{}\nFile: {}\nLine: {}", message, __FILE__, __LINE__)
    #else
        #define BE_EXCEPTION_MESSAGE(message) message
    #endif
#endif

#define BE_THROW(message)        throw std::runtime_error(BE_EXCEPTION_MESSAGE(message))
#define BE_THROW_FROM(exception) throw std::runtime_error(BE_EXCEPTION_MESSAGE(exception.what()));

#ifdef BE_PLATFORM_WINDOWS
    #define BE_DISPLAY_ERROR(error)                       \
        std::filesystem::path errorMessage{error.what()}; \
        MessageBox(NULL, errorMessage.wstring().c_str(), nullptr, MB_OK);
#endif
} // namespace be::internals
