#include "WindowsMouseEventsTest.h"

#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Windows/Windows/WindowsWindow.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    TEST_F(WindowsMouseEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseMoveMessage)
    {
        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedMouseMovementCoords = IntVec2(250, 500);

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedMouseMovementCoords, event.GetCoordinates());
        };

        const auto winApiMessageToSend = WM_MOUSEMOVE;

        auto sut = GetWindow();
        sut->SetMouseEventsHandler(expectedHandler);

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseMovementCoords.x, expectedMouseMovementCoords.y);
        PostMessage(sut->GetNativeHandle(), winApiMessageToSend, NULL, messageMouseCoords);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    /******************************************************/
    /*************** WindowsMouseWheelEventsTest **************/
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseWheelMessagesTest,
        WindowsMouseWheelEventsTest,
        testing::Values(120, 80, 500, -120, -80, 500
        )
    );

    TEST_P(WindowsMouseWheelEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseWheelMessageWithPositiveDelta)
    {
        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_SCROLLED;
        const auto expectedMouseMovementCoords = IntVec2(250, 500);
        const auto expectedWheelDelta = GetParam();

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedMouseMovementCoords, event.GetCoordinates());
            ASSERT_EQ(expectedWheelDelta, event.GetScrollValue());
        };

        const auto winApiMessageToSend = WM_MOUSEWHEEL;

        auto sut = GetWindow();
        sut->SetMouseEventsHandler(expectedHandler);

        const WPARAM messageMouseWheelDelta = MAKEWPARAM(0 /*low order*/, expectedWheelDelta);
        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseMovementCoords.x, expectedMouseMovementCoords.y);
        PostMessage(sut->GetNativeHandle(), winApiMessageToSend, messageMouseWheelDelta, messageMouseCoords);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /************ WindowsMouseButtonsDownEventsTest ************/
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseButtonsDownMessagesTest,
        WindowsMouseButtonsDownEventsTest,
        testing::Values(
            WindowsMouseButtonEventsTestParams{WM_LBUTTONDOWN, MouseButtonCode::BUTTON_LEFT},
            WindowsMouseButtonEventsTestParams{WM_MBUTTONDOWN, MouseButtonCode::BUTTON_MIDDLE},
            WindowsMouseButtonEventsTestParams{WM_RBUTTONDOWN, MouseButtonCode::BUTTON_RIGHT},
            WindowsMouseButtonEventsTestParams{WM_XBUTTONDOWN, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            WindowsMouseButtonEventsTestParams{WM_XBUTTONDOWN, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}
        )
    );

    TEST_P(WindowsMouseButtonsDownEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonDownMessages)
    {
        const WindowsMouseButtonEventsTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_BUTTON_PRESSED;
        const auto expectedButtonCode = testParams.expectedButtonCode;
        const auto expectedMouseClickCoords = IntVec2(400, 300);

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetButton());
            ASSERT_EQ(expectedMouseClickCoords, event.GetCoordinates());
        };

        auto sut = GetWindow();
        sut->SetMouseEventsHandler(expectedHandler);

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseClickCoords.x, expectedMouseClickCoords.y);
        PostMessage(sut->GetNativeHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, messageMouseCoords);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(WindowsMouseButtonsDownEventsTest, ProcessInputWillCaptureWindowWhenMouseButtonsDownMessagesAreSent)
    {
        const WindowsMouseButtonEventsTestParams testParams = GetParam();
        auto sut = GetWindow();

        bool wasHandlerCalled = false;
        const auto expectedCapturedWindowHandle = sut->GetNativeHandle();
        MouseEventHandler expectedHandler = [&](const MouseEvent&) {
            wasHandlerCalled = true;
            ASSERT_EQ(expectedCapturedWindowHandle, GetCapture());
        };
        sut->SetMouseEventsHandler(expectedHandler);

        PostMessage(sut->GetNativeHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, NULL);
        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /************ WindowsMouseButtonsUpEventsTest ************/
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseButtonsUpMessagesTest,
        WindowsMouseButtonsUpEventsTest,
        testing::Values(
            WindowsMouseButtonEventsTestParams{WM_LBUTTONUP, MouseButtonCode::BUTTON_LEFT},
            WindowsMouseButtonEventsTestParams{WM_MBUTTONUP, MouseButtonCode::BUTTON_MIDDLE},
            WindowsMouseButtonEventsTestParams{WM_RBUTTONUP, MouseButtonCode::BUTTON_RIGHT},
            WindowsMouseButtonEventsTestParams{WM_XBUTTONUP, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            WindowsMouseButtonEventsTestParams{WM_XBUTTONUP, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}
        )
    );

    TEST_P(WindowsMouseButtonsUpEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonUpMessages)
    {
        const WindowsMouseButtonEventsTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_BUTTON_RELEASED;
        const auto expectedButtonCode = testParams.expectedButtonCode;
        const auto expectedMouseClickCoords = IntVec2(400, 300);

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetButton());
            ASSERT_EQ(expectedMouseClickCoords, event.GetCoordinates());
        };

        auto sut = GetWindow();
        sut->SetMouseEventsHandler(expectedHandler);

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseClickCoords.x, expectedMouseClickCoords.y);
        PostMessage(sut->GetNativeHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, messageMouseCoords);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(WindowsMouseButtonsUpEventsTest, ProcessInputWillReleaseWindowWhenMouseButtonsUpMessagesAreSent)
    {
        const WindowsMouseButtonEventsTestParams testParams = GetParam();
        bool wasHandlerCalled = false;

        auto sut = GetWindow();
        const auto nativeWindowHandle = sut->GetNativeHandle();

        MouseEventHandler expectedHandler = [&](const MouseEvent&) {
            wasHandlerCalled = true;
            ASSERT_NE(nativeWindowHandle, GetCapture());
        };
        sut->SetMouseEventsHandler(expectedHandler);

        SetCapture(nativeWindowHandle);
        PostMessage(nativeWindowHandle, testParams.winApiMessageToSend, testParams.wParamToSend, NULL);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
    /******************************************************/
    /******************************************************/
} // namespace be::tests::integration
#endif
