#ifdef BE_ENABLE_WINDOWS_TESTS
    #include <Integration/Beast/Windows/Win32/Win32TestSetup.h>

    #include <Beast/Input/Events/Events.h>

namespace be::tests::integration
{
    struct WindowsKeyboardEventsTestParams
    {
        WPARAM virtualKeyCode;
        KeyCode expectedKeyCode;
    };

    class KeyboardEventsTest
        : public Win32Test,
          public ::testing::WithParamInterface<WindowsKeyboardEventsTestParams>
    {
    protected:
    };
} // namespace be::tests::integration
#endif
