#pragma once

namespace be::internals
{
#ifndef CLASS_CONSTRUCTORS_IMPLEMENTATION
    #define CLASS_CONSTRUCTORS_IMPLEMENTATION(ClassName, definition)  \
        ClassName(const ClassName&) = definition;                     \
        ClassName(ClassName&&) = definition;                          \
        ClassName& operator=(const ClassName&) noexcept = definition; \
        ClassName& operator=(ClassName&&) noexcept = definition;
#endif

#ifndef IMPLEMENT_CONSTRUCTORS_DELETED
    #define IMPLEMENT_CONSTRUCTORS_DELETED(ClassName) CLASS_CONSTRUCTORS_IMPLEMENTATION(ClassName, delete)
#endif

#ifndef IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE
    #define IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ClassName) \
        ClassName() = delete;                            \
        ~ClassName() = delete;                           \
        IMPLEMENT_CONSTRUCTORS_DELETED(ClassName)
#endif

#ifndef IMPLEMENT_CONSTRUCTORS_DEFAULT
    #define IMPLEMENT_CONSTRUCTORS_DEFAULT(ClassName) CLASS_CONSTRUCTORS_IMPLEMENTATION(ClassName, default)
#endif
} // namespace be::internals
