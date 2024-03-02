#ifdef BE_ENABLE_WINDOWS_TESTS
    #include <Integration/Beast/Windows/Win32/Win32TestSetup.h>

    #include <Beast/Input/Events/Events.h>
    #include <Beast/Common/Types.h>

namespace be::tests::integration
{
    /**
     * Class for general mouse messages/events tests.
     */
    class WindowsMouseEventsTest : public Win32Test
    {
    protected:
    };

    class WindowsMouseWheelEventsTest
        : public WindowsMouseEventsTest,
          public ::testing::WithParamInterface<int32>
    {
    protected:
    };

    struct WindowsMouseButtonEventsTestParams
    {
        uint32 winApiMessageToSend;
        MouseButtonCode expectedButtonCode;
        WPARAM wParamToSend = NULL;
    };

    /**
     * Class for mouse buttons down messages/events tests.
     */
    class WindowsMouseButtonsDownEventsTest
        : public WindowsMouseEventsTest,
          public ::testing::WithParamInterface<WindowsMouseButtonEventsTestParams>
    {
    protected:
    };

    /**
     * Class for mouse buttons up messages/events tests.
     */
    class WindowsMouseButtonsUpEventsTest
        : public WindowsMouseEventsTest,
          public ::testing::WithParamInterface<WindowsMouseButtonEventsTestParams>
    {
    protected:
    };
} // namespace be::tests::integration
#endif
