#include "WinTestSetup.h"
#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Types.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    struct WindowsKeyboardEventsTestParams
    {
        WPARAM virtualKeyCode;
        KeyCode expectedKeyCode;
    };

    class KeyboardEventsTest
        : public WinTest,
          public ::testing::WithParamInterface<WindowsKeyboardEventsTestParams>
    {
    protected:
    };
} // namespace be::tests::functional
#endif
