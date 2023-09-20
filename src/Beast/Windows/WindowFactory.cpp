#include <Beast/Windows/WindowFactory.h>

#ifdef BE_PLATFORM_WINDOWS
    #include <Beast/Windows/Win32/Win32Window.h>
    #include <Beast/Common/IdGenerators/UuId4Generator.h>
    #include <string>
#endif

#include <Beast/Common/Exceptions.h>

namespace be::internals
{
    Unique<IWindow> WindowFactory::Create(const WindowDescriptor& descriptor)
    {
#ifdef BE_PLATFORM_WINDOWS
        const std::wstring className =
            std::to_wstring(ToUnderlying(UuId4Generator().Generate())) + L"_BeastEngineWindowClassName";

        return MakeUnique<Win32Window>(descriptor, className.c_str());
#else
        CT_THROW("Platform not supported! Could not create a window.");
#endif
    }
} // namespace be::internals
