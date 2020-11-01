#ifdef BE_ENABLE_WINDOWS_TESTS
    #include "WinTestSetup.h"

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
} // namespace be::tests::integration
#endif
