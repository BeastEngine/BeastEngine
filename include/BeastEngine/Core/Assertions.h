#pragma once
#include "BeastEngine/Core/Logging.h"
#include "BeastEngine/Core/Debugging.h"

namespace be::internals
{
#ifndef BE_DEBUG_BREAK
    #ifdef _MSC_VER
        #define BE_DEBUG_BREAK() DebugBreak()
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
            }
    #else
        #define BE_ASSERT(expression)
    #endif
#endif
}; // namespace be::internals
