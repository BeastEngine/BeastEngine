#pragma once
#include "BeastEngine/Core/PlatformSetup.h"
#include "BeastEngine/BeastEngine.h"

#ifndef BE_MAIN
    #ifdef BE_PLATFORM_WINDOWS
        #include <Windows.h>
        #define BE_MAIN() int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
    #else
        #define BE_MAIN() int main(int argc, char** argv)
    #endif
#endif

namespace be
{
    /**
     * Creats AApplication instance.
     */
    extern UniquePtr<AApplication> CreateApplication();
} // namespace be

BE_MAIN()
{
    auto application = be::CreateApplication();
    application->Run();

    return 0;
}
