#pragma once
#include "BeastEngine/Core/PlatformSetup.h"
#include "BeastEngine/BeastEngine.h"

#ifndef BE_MAIN
    #ifdef BE_PLATFORM_WINDOWS
        #define BE_MAIN() int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
        #define BE_WINDOW_HANDLE_INSTANCE be::WindowHandleInstance(hInstance)
    #else
        #define BE_MAIN() int main(int argc, char** argv)
    #endif
#endif

namespace be
{
    /**
     * Creates AApplication instance.
     *  Definition must be provided by the user!
     * 
     * @param WindowHandleInstance Handle to be used as parent window handle provided by the OS
     * 
     * @return Pointer to the implementation of the AApplication instance
     */
    extern UniquePtr<AApplication> CreateApplication(WindowHandleInstance windowHandleInstance);
} // namespace be

BE_MAIN()
{
    try
    {
        auto application = be::CreateApplication(BE_WINDOW_HANDLE_INSTANCE);
        application->Run();
    }
    catch (const std::exception& ex)
    {
        printf(ex.what());
    }

    return 0;
}
