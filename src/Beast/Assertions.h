#pragma once
#include <Beast/Debugging.h>

namespace be::internals
{
#ifndef BE_DEBUG_BREAK
    #ifdef _MSC_VER
        #define BE_DEBUG_BREAK() DebugBreak()
    #else
        #define BE_DEBUG_BREAK()
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

#ifndef BE_ASSERT
    #ifdef BE_ASSERTIONS_ENABLED
        #define BE_ASSERT(expression)                                                                          \
            if (expression)                                                                                    \
            { /* This is intentionally empty. Solitare 'if' could lead to potenial bugs */                     \
            }                                                                                                  \
            else                                                                                               \
            {                                                                                                  \
                BE_DEBUG_LOG_ERROR("The '{}' assertion failed! It evaluated to: {}", #expression, expression); \
                BE_DEBUG_BREAK();                                                                              \
            }
    #else
        #define BE_ASSERT(expression)
    #endif
#endif

#ifndef BE_ASSERT_MSG
    #ifdef BE_ASSERTIONS_ENABLED
        #define BE_ASSERT_MSG(expression, fmt, ...)                                        \
            if (expression)                                                                \
            { /* This is intentionally empty. Solitare 'if' could lead to potenial bugs */ \
            }                                                                              \
            else                                                                           \
            {                                                                              \
                BE_DEBUG_LOG_ERROR(fmt, __VA_ARGS__);                          \
                BE_DEBUG_BREAK();                                                          \
            }
    #else
        #define BE_ASSERT_MSG(expression, fmt, ...)
    #endif
#endif
}; // namespace be::internals
