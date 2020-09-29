#include "WindowsMouseEventsTest.h"

#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Windows/Windows/WindowsWindow.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
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
        testing::Values(120, 80, 500, -120, -80, 500));

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
            MouseButtonMessageTestParams{WM_LBUTTONDOWN, MouseButtonCode::BUTTON_LEFT},
            MouseButtonMessageTestParams{WM_MBUTTONDOWN, MouseButtonCode::BUTTON_MIDDLE},
            MouseButtonMessageTestParams{WM_RBUTTONDOWN, MouseButtonCode::BUTTON_RIGHT},
            MouseButtonMessageTestParams{WM_XBUTTONDOWN, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            MouseButtonMessageTestParams{WM_XBUTTONDOWN, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}));

    TEST_P(MouseButtonsDownMessagesTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonDownMessages)
    {
        const MouseButtonMessageTestParams testParams = GetParam();

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
        const MouseButtonMessageTestParams testParams = GetParam();

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
            MouseButtonMessageTestParams{WM_LBUTTONUP, MouseButtonCode::BUTTON_LEFT},
            MouseButtonMessageTestParams{WM_MBUTTONUP, MouseButtonCode::BUTTON_MIDDLE},
            MouseButtonMessageTestParams{WM_RBUTTONUP, MouseButtonCode::BUTTON_RIGHT},
            MouseButtonMessageTestParams{WM_XBUTTONUP, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            MouseButtonMessageTestParams{WM_XBUTTONUP, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}));

    TEST_P(MouseButtonsUpMessagesTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonUpMessages)
    {
        const MouseButtonMessageTestParams testParams = GetParam();

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
        const MouseButtonMessageTestParams testParams = GetParam();
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

    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_MouseButtonsHeldDownMessagesTest,
        MouseButtonsHeldDownMessagesTest,
        testing::Values(
            MouseButtonsHeldDownMessagesTestParams{MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP, MouseButtonCode::BUTTON_LEFT},
            MouseButtonsHeldDownMessagesTestParams{MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP, MouseButtonCode::BUTTON_MIDDLE},
            MouseButtonsHeldDownMessagesTestParams{MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP, MouseButtonCode::BUTTON_RIGHT},
            MouseButtonsHeldDownMessagesTestParams{MOUSEEVENTF_XDOWN, MOUSEEVENTF_XUP, MouseButtonCode::BUTTON4, XBUTTON1},
            MouseButtonsHeldDownMessagesTestParams{MOUSEEVENTF_XDOWN, MOUSEEVENTF_XUP, MouseButtonCode::BUTTON5, XBUTTON2}));

    TEST_P(MouseButtonsHeldDownMessagesTest, ProcessInputWillProperlyDispatchMouseButtonHeldDownEventsWhenAppropriateMouseButtonIsPressed)
    {
        const MouseButtonsHeldDownMessagesTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN;
        const auto expectedButtonCode = testParams.expectedButtonCode;

        MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
            wasHandlerCalled = true;

            EXPECT_EQ(expectedEventType, event.GetType());
            EXPECT_EQ(expectedButtonCode, event.GetButton());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetMouseEventsHandler(expectedHandler);

        MOUSEINPUT inputData = {0};
        inputData.mouseData = testParams.additionalEventData;
        inputData.dwFlags = testParams.pressedVirtualKeyCode;

        INPUT inputs[] = {INPUT{INPUT_MOUSE, inputData}};
        SendInput(1, inputs, sizeof(inputs));

        sut.ProcessInput();
        EXPECT_TRUE(wasHandlerCalled);

        // Clear the input state
        {
            MOUSEINPUT mouseInput = {0};
            mouseInput.mouseData = testParams.additionalEventData;
            mouseInput.dwFlags = testParams.virtualKeyCodeToReset;

            INPUT clearingInputs[] = {INPUT{INPUT_MOUSE, mouseInput}};
            SendInput(1, clearingInputs, sizeof(clearingInputs));
            sut.ProcessInput();
        }
    }
} // namespace be::tests::integration
#endif
