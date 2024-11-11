#pragma once
#include "Beast/PlatformSetup.h"
#include "Beast/Application.h"

#include "Beast/Common/Types.h"

#ifndef BE_MAIN
    #ifdef BE_PLATFORM_WINDOWS
        #define BE_MAIN()                 int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
        #define BE_WINDOW_HANDLE_INSTANCE hInstance
    #else
        #define BE_MAIN() int main(int argc, char** argv)
    #endif
#endif

namespace be
{
    /**
     * @brief Creates AApplication instance.
     * Definition of this function must be provided by the user of the engine!
     * 
     * @param windowHandleInstance - Handle to be used as parent window handle provided by the OS
     * @return Pointer to the implementation of the AApplication instance
     */
    extern Unique<AApplication> CreateApplication(WindowHandleInstance windowHandleInstance);
} // namespace be

BE_MAIN()
{
    auto application = be::CreateApplication(BE_WINDOW_HANDLE_INSTANCE);
    application->Start();

    return 0;
}
