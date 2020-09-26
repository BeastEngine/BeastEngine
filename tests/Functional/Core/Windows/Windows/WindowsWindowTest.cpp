#ifdef BE_PLATFORM_WINDOWS
    #include "WindowsWindowTest.h"

    #include <BeastEngine/Core/Windows/Windows/WindowsWindow.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::functional
{
    TEST_F(MouseMessagesTest, ProcessInputWillProperlyHandleAndDispatchMouseMoveMessage)
    {
        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedMouseMovementCoords = IntVec2(250, 500);

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedMouseMovementCoords, event.GetCoordinates());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        const auto winApiMessageToSend = WM_MOUSEMOVE;

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetMouseEventsHandler(expectedHandler);

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseMovementCoords.x, expectedMouseMovementCoords.y);
        PostMessage(sut.GetNativeHandle(), winApiMessageToSend, NULL, messageMouseCoords);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    /******************************************************/
    /*************** MouseWheelMessagesTest **************/
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseWheelMessagesTest,
        MouseWheelMessagesTest,
        testing::Values(120, 80, 500, -120, -80, 500)
    );
    
    TEST_P(MouseWheelMessagesTest, ProcessInputWillProperlyHandleAndDispatchMouseWheelMessageWithPositiveDelta)
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

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        const auto winApiMessageToSend = WM_MOUSEWHEEL;

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetMouseEventsHandler(expectedHandler);

        const WPARAM messageMouseWheelDelta = MAKEWPARAM(0 /*low order*/, expectedWheelDelta);
        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseMovementCoords.x, expectedMouseMovementCoords.y);
        PostMessage(sut.GetNativeHandle(), winApiMessageToSend, messageMouseWheelDelta, messageMouseCoords);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /************ MouseButtonsDownMessagesTest ************/
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseButtonsDownMessagesTest,
        MouseButtonsDownMessagesTest,
        testing::Values(
            MouseButtonMessageTestParam{WM_LBUTTONDOWN, MouseButtonCode::BUTTON_LEFT},
            MouseButtonMessageTestParam{WM_MBUTTONDOWN, MouseButtonCode::BUTTON_MIDDLE},
            MouseButtonMessageTestParam{WM_RBUTTONDOWN, MouseButtonCode::BUTTON_RIGHT},
            MouseButtonMessageTestParam{WM_XBUTTONDOWN, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            MouseButtonMessageTestParam{WM_XBUTTONDOWN, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}
        )
    );

    TEST_P(MouseButtonsDownMessagesTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonDownMessages)
    {
        const MouseButtonMessageTestParam testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_BUTTON_PRESSED;
        const auto expectedMouseButtonCode = testParams.expectedButtonCode;
        const auto expectedMouseClickCoords = IntVec2(400, 300);

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedMouseButtonCode, event.GetButton());
            ASSERT_EQ(expectedMouseClickCoords, event.GetCoordinates());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetMouseEventsHandler(expectedHandler);

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseClickCoords.x, expectedMouseClickCoords.y);
        PostMessage(sut.GetNativeHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, messageMouseCoords);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(MouseButtonsDownMessagesTest, ProcessInputWillCaptureWindowWhenMouseButtonsDownMessagesAreSent)
    {
        const MouseButtonMessageTestParam testParams = GetParam();

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);

        bool wasHandlerCalled = false;
        const auto expectedCapturedWindowHandle = sut.GetNativeHandle();
        MouseEventHandler expectedHandler = [&](const MouseEvent&) {
            wasHandlerCalled = true;
            ASSERT_EQ(expectedCapturedWindowHandle, GetCapture());
        };
        sut.SetMouseEventsHandler(expectedHandler);

        PostMessage(sut.GetNativeHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, NULL);
        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /************ MouseButtonsUpMessagesTest ************/
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseButtonsUpMessagesTest,
        MouseButtonsUpMessagesTest,
        testing::Values(
            MouseButtonMessageTestParam{WM_LBUTTONUP, MouseButtonCode::BUTTON_LEFT},
            MouseButtonMessageTestParam{WM_MBUTTONUP, MouseButtonCode::BUTTON_MIDDLE},
            MouseButtonMessageTestParam{WM_RBUTTONUP, MouseButtonCode::BUTTON_RIGHT},
            MouseButtonMessageTestParam{WM_XBUTTONUP, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            MouseButtonMessageTestParam{WM_XBUTTONUP, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}
        )
    );

    TEST_P(MouseButtonsUpMessagesTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonUpMessages)
    {
        const MouseButtonMessageTestParam testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_BUTTON_RELEASED;
        const auto expectedMouseButtonCode = testParams.expectedButtonCode;
        const auto expectedMouseClickCoords = IntVec2(400, 300);

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedMouseButtonCode, event.GetButton());
            ASSERT_EQ(expectedMouseClickCoords, event.GetCoordinates());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetMouseEventsHandler(expectedHandler);

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseClickCoords.x, expectedMouseClickCoords.y);
        PostMessage(sut.GetNativeHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, messageMouseCoords);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(MouseButtonsUpMessagesTest, ProcessInputWillReleaseWindowWhenMouseButtonsUpMessagesAreSent)
    {
        const MouseButtonMessageTestParam testParams = GetParam();
        bool wasHandlerCalled = false;

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        const auto nativeWindowHandle = sut.GetNativeHandle();

        MouseEventHandler expectedHandler = [&](const MouseEvent&) {
            wasHandlerCalled = true;
            ASSERT_NE(nativeWindowHandle, GetCapture());
        };
        sut.SetMouseEventsHandler(expectedHandler);

        SetCapture(nativeWindowHandle);
        PostMessage(nativeWindowHandle, testParams.winApiMessageToSend, testParams.wParamToSend, NULL);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
    /******************************************************/
    /******************************************************/
} // namespace be::tests::functional
#endif
