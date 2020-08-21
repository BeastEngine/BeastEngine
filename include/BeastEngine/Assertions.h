#pragma once
#include "BeastEngine/Logging.h"

namespace be::internals
{
#ifndef BE_DEBUG_BREAK
    #ifdef _MSC_VER
        #define BE_DEBUG_BREAK() DebugBreak()
    #endif
#endif

#ifndef BE_CRITICAL_ASSERT
    #ifdef BE_ASSERTIONS_ENABLED
        #define BE_CRITICAL_ASSERT(expression)                                                \
            if (expression) {}                                                                \
            else                                                                              \
            {                                                                                 \
                BE_DEBUG_LOG_FATAL_ERROR("The critical '{}' assertion failed!", #expression); \
                BE_DEBUG_BREAK();                                                             \
            }
    #else
        #define BE_CRITICAL_ASSERT(expression)
    #endif
#endif

#ifndef BE_ASSERT
    #ifdef BE_ASSERTIONS_ENABLED
        #define BE_ASSERT(expression)                                          \
            if (expression) {}                                                 \
            else                                                               \
            {                                                                  \
                BE_DEBUG_LOG_ERROR("The '{}' assertion failed!", #expression); \
            }
    #else
        #define BE_ASSERT(expression)
    #endif
#endif
}; // namespace be::internals
