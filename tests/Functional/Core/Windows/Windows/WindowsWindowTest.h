#ifdef BE_PLATFORM_WINDOWS
    #include "TestSetup.h"

    #include <BeastEngine/Core/Types.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::functional
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
 
    struct MouseButtonMessageTestParam
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
          public ::testing::WithParamInterface<MouseButtonMessageTestParam>
    {
    protected:
    };

    /**
     * Class for mouse buttons up messages/events tests.
     */
    class MouseButtonsUpMessagesTest
        : public MouseMessagesTest,
          public ::testing::WithParamInterface<MouseButtonMessageTestParam>
    {
    protected:
    };
} // namespace be::tests::functional
#endif
