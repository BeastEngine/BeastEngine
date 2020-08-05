#pragma once

namespace be::internals
{
#ifndef CLASS_CONSTRUCTORS_IMPLEMENTATION
#    define CLASS_CONSTRUCTORS_IMPLEMENTATION(ClassName, definition) \
        ClassName(const ClassName&) = definition;                   \
        ClassName(ClassName&&) = definition;                        \
        const ClassName& operator=(const ClassName&) = definition;  \
        ClassName&& operator=(ClassName&&) = definition;
#endif // !CLASS_CONSTRUCTORS_IMPLEMENTATION

#ifndef IMPLEMENT_CONSTRUCTORS_DELETED
#    define IMPLEMENT_CONSTRUCTORS_DELETED(ClassName) \
        CLASS_CONSTRUCTORS_IMPLEMENTATION(ClassName, delete)
#endif // !IMPLEMENT_CONSTRUCTORS_DELETED

#ifndef IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE
#    define IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE(ClassName) \
        ClassName() = delete;                             \
        ~ClassName() = delete;                            \
        IMPLEMENT_CONSTRUCTORS_DELETED(ClassName)
#endif // !IMPLEMENT_CLASS_NOT_CONSTRUCTIBLE

#ifndef IMPLEMENT_CONSTRUCTORS_DEFAULT
#    define IMPLEMENT_CONSTRUCTORS_DEFAULT(ClassName) \
        CLASS_CONSTRUCTORS_IMPLEMENTATION(ClassName, default)
#endif // !IMPLEMENT_CONSTRUCTORS_DEFAULT
} // namespace be::internals
