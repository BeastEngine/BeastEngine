#pragma once
#include <format>

namespace be
{
#ifndef BE_EXCEPTION_MESSAGE
    #ifdef BE_DEBUG
        #define BE_EXCEPTION_MESSAGE(message) std::format("{}\nFile: {}\nLine: {}", message, __FILE__, __LINE__)
    #else
        #define BE_EXCEPTION_MESSAGE(message) message
    #endif
#endif

#ifndef BE_THROW
    #define BE_THROW(message)        throw std::runtime_error(BE_EXCEPTION_MESSAGE(message))
    #define BE_THROW_FROM(exception) throw std::runtime_error(BE_EXCEPTION_MESSAGE(exception.what()));
#endif
} // namespace be
