#ifdef BE_ENABLE_WINDOWS_TESTS
    #include <Integration/Beast/Windows/Win32/Win32MouseEventsTest.h>

    #include <Beast/Windows/Win32/Win32Window.h>
    #include <Beast/Input/Events/Events.h>

namespace be::tests::integration
{
    TEST_F(WindowsMouseEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseMovedMessage)
    {
        const auto expectedPosition = Vec2i(250, 500);

        auto sut = GetWindow();
        const auto& input = sut->GetInputHandler();

        const Vec2i positionBefore = input.GetMousePosition();

        const LPARAM messageMouseCoords = MAKELPARAM(expectedPosition.x, expectedPosition.y);
        PostMessage(sut->GetHandle(), WM_MOUSEMOVE, NULL, messageMouseCoords);
        sut->ProcessInput();
        
        const auto& positionAfter = input.GetMousePosition();
        ASSERT_NE(positionBefore, positionAfter);
        ASSERT_EQ(expectedPosition, positionAfter);
    }

    /******************************************************/
    /*************** WindowsMouseWheelEventsTest **************/
    //INSTANTIATE_TEST_SUITE_P(
    //    Win32WindowTest_MouseWheelMessagesTest,
    //    WindowsMouseWheelEventsTest,
    //    testing::Values(120, 80, 500, -120, -80, 500)
    //);

    //TEST_P(WindowsMouseWheelEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseWheelMessageWithPositiveDelta)
    //{
    //    bool wasHandlerCalled = false;
    //    const auto expectedEventType = MouseEventType::EVENT_MOUSE_SCROLLED;
    //    const auto expectedMouseMovementCoords = Vec2i(250, 500);
    //    const auto expectedWheelDelta = GetParam();

    //    MouseEventHandler expectedHandler = [&](const MouseEvent& event) {
    //        wasHandlerCalled = true;

    //        ASSERT_EQ(expectedEventType, event.GetType());
    //        ASSERT_EQ(expectedMouseMovementCoords, event.GetMousePosition());
    //        ASSERT_EQ(expectedWheelDelta, event.GetScrollValue());
    //    };

    //    const auto winApiMessageToSend = WM_MOUSEWHEEL;

    //    auto sut = GetWindow();
    //    sut->SetMouseEventsHandler(expectedHandler);

    //    const WPARAM messageMouseWheelDelta = MAKEWPARAM(0 /*low order*/, expectedWheelDelta);
    //    const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseMovementCoords.x, expectedMouseMovementCoords.y);
    //    PostMessage(sut->GetHandle(), winApiMessageToSend, messageMouseWheelDelta, messageMouseCoords);

    //    sut->ProcessInput();
    //    ASSERT_TRUE(wasHandlerCalled);
    //}
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /************ WindowsMouseButtonsDownEventsTest ************/
    INSTANTIATE_TEST_SUITE_P(
        Win32WindowTest_MouseButtonsDownMessagesTest,
        WindowsMouseButtonsDownEventsTest,
        testing::Values(
            WindowsMouseButtonDownEventsTestParams{WM_LBUTTONDOWN, MouseButtonCode::BUTTON_LEFT},
            WindowsMouseButtonDownEventsTestParams{WM_MBUTTONDOWN, MouseButtonCode::BUTTON_MIDDLE},
            WindowsMouseButtonDownEventsTestParams{WM_RBUTTONDOWN, MouseButtonCode::BUTTON_RIGHT},
            WindowsMouseButtonDownEventsTestParams{WM_XBUTTONDOWN, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            WindowsMouseButtonDownEventsTestParams{WM_XBUTTONDOWN, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}
        )
    );

    TEST_P(WindowsMouseButtonsDownEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonDownMessages)
    {
        const WindowsMouseButtonDownEventsTestParams testParams = GetParam();

        const auto mouseButton = testParams.expectedButtonCode;
        const auto expectedMouseClickCoords = Vec2i(400, 300);

        auto sut = GetWindow();
        const auto& input = sut->GetInputHandler();

        ASSERT_FALSE(input.IsMouseButtonDown(mouseButton));
        ASSERT_FALSE(input.IsMouseButtonHeldDown(mouseButton));
        ASSERT_FALSE(input.IsMouseButtonPressed(mouseButton));

        const LPARAM messageMouseCoords = MAKELPARAM(expectedMouseClickCoords.x, expectedMouseClickCoords.y);
        PostMessage(sut->GetHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, messageMouseCoords);
        sut->ProcessInput();
        
        ASSERT_TRUE(input.IsMouseButtonDown(mouseButton));
        ASSERT_FALSE(input.IsMouseButtonHeldDown(mouseButton));
        ASSERT_TRUE(input.IsMouseButtonPressed(mouseButton));
    }

    TEST_P(WindowsMouseButtonsDownEventsTest, ProcessInputWillCaptureWindowWhenMouseButtonsDownMessagesAreSent)
    {
        const WindowsMouseButtonDownEventsTestParams testParams = GetParam();
        
        auto sut = GetWindow();
        const auto expectedCapturedWindowHandle = sut->GetHandle();

        PostMessage(sut->GetHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, NULL);
        sut->ProcessInput();
        
        ASSERT_EQ(expectedCapturedWindowHandle, GetCapture());
    }
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /************ WindowsMouseButtonsUpEventsTest ************/
    INSTANTIATE_TEST_SUITE_P(
        Win32WindowTest_MouseButtonsUpMessagesTest,
        WindowsMouseButtonsUpEventsTest,
        testing::Values(
            WindowsMouseButtonUpEventsTestParams{WM_LBUTTONDOWN, WM_LBUTTONUP, MouseButtonCode::BUTTON_LEFT},
            WindowsMouseButtonUpEventsTestParams{WM_MBUTTONDOWN, WM_MBUTTONUP, MouseButtonCode::BUTTON_MIDDLE},
            WindowsMouseButtonUpEventsTestParams{WM_RBUTTONDOWN, WM_RBUTTONUP, MouseButtonCode::BUTTON_RIGHT},
            WindowsMouseButtonUpEventsTestParams{WM_XBUTTONDOWN, WM_XBUTTONUP, MouseButtonCode::BUTTON4, MAKEWPARAM(0 /*low order*/, XBUTTON1)},
            WindowsMouseButtonUpEventsTestParams{WM_XBUTTONDOWN, WM_XBUTTONUP, MouseButtonCode::BUTTON5, MAKEWPARAM(0, XBUTTON2)}
        )
    );

    TEST_P(WindowsMouseButtonsUpEventsTest, ProcessInputWillProperlyHandleAndDispatchMouseButtonUpMessages)
    {
        const WindowsMouseButtonUpEventsTestParams testParams = GetParam();

        const auto mouseButton = testParams.expectedButtonCode;
        const auto expectedPosition = Vec2i(400, 300);

        auto sut = GetWindow();
        const auto& input = sut->GetInputHandler();

        PostMessage(sut->GetHandle(), testParams.initialStateMessage, testParams.wParamToSend, 0);
        sut->ProcessInput();

        ASSERT_TRUE(input.IsMouseButtonDown(mouseButton));
        ASSERT_TRUE(input.IsMouseButtonPressed(mouseButton));
        ASSERT_NE(expectedPosition, input.GetMousePosition());

        const LPARAM messageMouseCoords = MAKELPARAM(expectedPosition.x, expectedPosition.y);
        PostMessage(sut->GetHandle(), testParams.winApiMessageToSend, testParams.wParamToSend, messageMouseCoords);

        sut->ProcessInput();
        ASSERT_FALSE(input.IsMouseButtonDown(mouseButton));
        ASSERT_FALSE(input.IsMouseButtonPressed(mouseButton));
        ASSERT_EQ(expectedPosition, input.GetMousePosition());
    }

    TEST_P(WindowsMouseButtonsUpEventsTest, ProcessInputWillReleaseWindowWhenMouseButtonsUpMessagesAreSent)
    {
        const WindowsMouseButtonUpEventsTestParams testParams = GetParam();

        auto sut = GetWindow();
        const auto nativeWindowHandle = sut->GetHandle();

        SetCapture(nativeWindowHandle);
        PostMessage(nativeWindowHandle, testParams.winApiMessageToSend, testParams.wParamToSend, NULL);

        sut->ProcessInput();
        ASSERT_NE(nativeWindowHandle, GetCapture());
    }
    /******************************************************/
    /******************************************************/
} // namespace be::tests::integration
#endif
