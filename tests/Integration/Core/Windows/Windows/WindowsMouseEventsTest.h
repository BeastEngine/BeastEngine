#include "WinTestSetup.h"

#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Types.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    /**
     * Class for general mouse messages/events tests.
     */
    class MouseMessagesTest : public ::testing::Test
    {
    protected:
    };

    class MouseWheelMessagesTest
        : public MouseMessagesTest,
          public ::testing::WithParamInterface<int32>
    {
    protected:
    };

    struct MouseButtonMessageTestParams
    {
        uint32 winApiMessageToSend;
        MouseButtonCode expectedButtonCode;
        WPARAM wParamToSend = NULL;
    };

    /**
     * Class for mouse buttons down messages/events tests.
     */
    class MouseButtonsDownMessagesTest
        : public MouseMessagesTest,
          public ::testing::WithParamInterface<MouseButtonMessageTestParams>
    {
    protected:
    };

    /**
     * Class for mouse buttons up messages/events tests.
     */
    class MouseButtonsUpMessagesTest
        : public MouseMessagesTest,
          public ::testing::WithParamInterface<MouseButtonMessageTestParams>
    {
    protected:
    };

    struct MouseButtonsHeldDownMessagesTestParams
    {
        unsigned long pressedVirtualKeyCode;
        unsigned long virtualKeyCodeToReset;
        MouseButtonCode expectedButtonCode;
        DWORD additionalEventData = NULL;
    };

    /**
     * Class for mouse buttons held down messages/events tests.
     */
    class MouseButtonsHeldDownMessagesTest
        : public MouseMessagesTest,
          public ::testing::WithParamInterface<MouseButtonsHeldDownMessagesTestParams>
    {
    protected:
    };
} // namespace be::tests::integration
#endif
