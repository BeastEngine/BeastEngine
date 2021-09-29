#include "BeastEngine/Core/Windows/WindowFactory.h"

#ifdef BE_PLATFORM_WINDOWS
    #include "BeastEngine/Core/Windows/Win32/Win32Window.h"
#endif

#include <Common/Exceptions.h>

namespace be::internals
{
    Unique<IWindow> WindowFactory::Create(const WindowDescriptor& descriptor)
    {
#ifdef BE_PLATFORM_WINDOWS
        return CreateUnique<Win32Window>(descriptor, L"BeastEngineWindowClassName");
#else
        CT_THROW("Platform not supported! Could not create a window.");
#endif
    }
} // namespace be::internals
