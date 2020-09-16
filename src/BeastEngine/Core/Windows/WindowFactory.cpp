#include "BeastEngine/Core/Windows/WindowFactory.h"

#ifdef BE_PLATFORM_WINDOWS
    #include "BeastEngine/Core/Windows/Windows/WindowsWindow.h"
#endif

namespace be::internals
{
    UniquePtr<IWindow> WindowFactory::Create(const WindowDescriptor& descriptor)
    {
#ifdef BE_PLATFORM_WINDOWS
        return CreateUniquePtr<WindowsWindow>(descriptor);
#else
        return nullptr;
#endif
    }
} // namespace be::internals
