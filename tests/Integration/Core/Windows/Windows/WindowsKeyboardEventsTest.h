#include "WinTestSetup.h"
#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Types.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    struct KeyboardMessageTestParams
    {
        WPARAM virtualKeyCode;
        KeyCode expectedKeyCode;
    };

    /**
     * Class for general mouse messages/events tests.
     */
    class KeyboardMessagesTest
        : public ::testing::Test,
          public ::testing::WithParamInterface<KeyboardMessageTestParams>
    {
    protected:
    };
} // namespace be::tests::functional
#endif
