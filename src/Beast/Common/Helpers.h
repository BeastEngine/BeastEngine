#pragma once

namespace be
{
#ifndef CT_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION
    #define CT_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION(ClassName, definition) \
        ClassName(const ClassName&) = definition;                                  \
        ClassName(ClassName&&) = definition;                                       \
        ClassName& operator=(const ClassName&) noexcept = definition;              \
        ClassName& operator=(ClassName&&) noexcept = definition;
#endif

#ifndef CT_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED
    #define CT_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(ClassName) CT_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION(ClassName, delete)
#endif

#ifndef CT_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE
    #define CT_IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ClassName) \
        ClassName() = delete;                               \
        ~ClassName() = delete;                              \
        CT_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(ClassName)
#endif

#ifndef CT_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT
    #define CT_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT(ClassName) CT_CLASS_ADDITIONAL_CONSTRUCTORS_IMPLEMENTATION(ClassName, default)
#endif
} // namespace be
