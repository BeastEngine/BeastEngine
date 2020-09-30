#include "WinTestSetup.h"

#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Types.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    /**
     * Class for general mouse messages/events tests.
     */
    class WindowsMouseEventsTest : public WinTest
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
