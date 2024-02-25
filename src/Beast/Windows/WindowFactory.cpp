#include "Beast/Windows/WindowFactory.h"

#ifdef BE_PLATFORM_WINDOWS
    #include "Beast/Windows/Win32/Win32Window.h"
    #include "Beast/Common/UUIDGenerator.h"

    #include <string>
#endif

#include "Beast/Debug.h"

namespace be::internals
{
    Unique<IWindow> WindowFactory::Create(const WindowDescriptor& descriptor)
    {
#ifdef BE_PLATFORM_WINDOWS
        const std::wstring className =
            std::to_wstring(GenerateUUID4().Raw()) + L"_BeastEngineWindowClassName";

        return MakeUnique<Win32Window>(descriptor, className.c_str());
#else
        BE_THROW("Platform not supported! Could not create a window.");
#endif
    }
} // namespace be::internals
