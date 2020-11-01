#pragma once

namespace be::internals
{
#ifndef BE_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION
    #define BE_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION(ClassName, definition) \
        ClassName(const ClassName&) = definition;                                  \
        ClassName(ClassName&&) = definition;                                       \
        ClassName& operator=(const ClassName&) noexcept = definition;              \
        ClassName& operator=(ClassName&&) noexcept = definition;
#endif

#ifndef BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED
    #define BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(ClassName) BE_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION(ClassName, delete)
#endif

#ifndef BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE
    #define BE_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ClassName) \
        ClassName() = delete;                               \
        ~ClassName() = delete;                              \
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(ClassName)
#endif

#ifndef BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT
    #define BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT(ClassName) BE_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION(ClassName, default)
#endif
} // namespace be::internals
