#pragma once
#include "Beast/PlatformSetup.h"
#include "Beast/Application.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Filesystem/Types.h"

#ifndef BE_MAIN
    #ifdef BE_PLATFORM_WINDOWS
        #define BE_MAIN()                 int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int)
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
     * @param cwd - Current working directory of the running application
     * 
     * @return Pointer to the implementation of the AApplication instance
     */
    extern Unique<AApplication> CreateApplication(WindowHandleInstance windowHandleInstance, const fs::Path& cwd);
} // namespace be

BE_MAIN()
{
    be::fs::Path exePath = "";

#ifdef BE_PLATFORM_WINDOWS
    constexpr uint16_t NTFS_MAX_PATH_LENGTH = 32768;
    wchar_t exeNameBuffer[NTFS_MAX_PATH_LENGTH];

    const DWORD fileNameLength = GetModuleFileNameW(NULL, exeNameBuffer, NTFS_MAX_PATH_LENGTH);
    if (fileNameLength == 0)
    {
        return -1;
    }
    else if (fileNameLength >= NTFS_MAX_PATH_LENGTH)
    {
        return -1;
    }

    exePath = be::fs::Path(exeNameBuffer);
    exePath = exePath.parent_path();
#else
    exePath = argv[0];
    exePath = exePath.parent_path();
#endif
    const be::Unique<be::AApplication> application = be::CreateApplication(BE_WINDOW_HANDLE_INSTANCE, exePath);
    application->Start();

    return 0;
}
