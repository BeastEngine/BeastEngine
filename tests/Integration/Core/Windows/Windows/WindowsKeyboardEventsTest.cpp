#include "WindowsKeyboardEventsTest.h"

#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Windows/Windows/WindowsWindow.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_KeyboardMessagesTest,
        KeyboardEventsTest,
        testing::Values(
            WindowsKeyboardEventsTestParams{0x41, KeyCode::A},
            WindowsKeyboardEventsTestParams{0x42, KeyCode::B},
            WindowsKeyboardEventsTestParams{0x43, KeyCode::C},
            WindowsKeyboardEventsTestParams{0x44, KeyCode::D},
            WindowsKeyboardEventsTestParams{0x45, KeyCode::E},
            WindowsKeyboardEventsTestParams{0x46, KeyCode::F},
            WindowsKeyboardEventsTestParams{0x47, KeyCode::G},
            WindowsKeyboardEventsTestParams{0x48, KeyCode::H},
            WindowsKeyboardEventsTestParams{0x49, KeyCode::I},
            WindowsKeyboardEventsTestParams{0x4A, KeyCode::J},
            WindowsKeyboardEventsTestParams{0x4B, KeyCode::K},
            WindowsKeyboardEventsTestParams{0x4C, KeyCode::L},
            WindowsKeyboardEventsTestParams{0x4D, KeyCode::M},
            WindowsKeyboardEventsTestParams{0x4E, KeyCode::N},
            WindowsKeyboardEventsTestParams{0x4F, KeyCode::O},
            WindowsKeyboardEventsTestParams{0x50, KeyCode::P},
            WindowsKeyboardEventsTestParams{0x51, KeyCode::Q},
            WindowsKeyboardEventsTestParams{0x52, KeyCode::R},
            WindowsKeyboardEventsTestParams{0x53, KeyCode::S},
            WindowsKeyboardEventsTestParams{0x54, KeyCode::T},
            WindowsKeyboardEventsTestParams{0x55, KeyCode::U},
            WindowsKeyboardEventsTestParams{0x56, KeyCode::V},
            WindowsKeyboardEventsTestParams{0x57, KeyCode::X},
            WindowsKeyboardEventsTestParams{0x58, KeyCode::Y},
            WindowsKeyboardEventsTestParams{0x59, KeyCode::Z},
            WindowsKeyboardEventsTestParams{VK_BACK, KeyCode::Backspace},
            WindowsKeyboardEventsTestParams{VK_TAB, KeyCode::Tab},
            WindowsKeyboardEventsTestParams{VK_CLEAR, KeyCode::Delete},
            WindowsKeyboardEventsTestParams{VK_RETURN, KeyCode::Enter},
            WindowsKeyboardEventsTestParams{VK_SHIFT, KeyCode::LeftShift},
            WindowsKeyboardEventsTestParams{VK_CONTROL, KeyCode::LeftControl},
            WindowsKeyboardEventsTestParams{VK_MENU, KeyCode::LeftAlt},
            WindowsKeyboardEventsTestParams{VK_PAUSE, KeyCode::Pause},
            WindowsKeyboardEventsTestParams{VK_CAPITAL, KeyCode::CapsLock},
            WindowsKeyboardEventsTestParams{VK_ESCAPE, KeyCode::Escape},
            WindowsKeyboardEventsTestParams{VK_SPACE, KeyCode::Space},
            WindowsKeyboardEventsTestParams{VK_PRIOR, KeyCode::PageUp},
            WindowsKeyboardEventsTestParams{VK_NEXT, KeyCode::PageDown},
            WindowsKeyboardEventsTestParams{VK_END, KeyCode::End},
            WindowsKeyboardEventsTestParams{VK_HOME, KeyCode::Home},
            WindowsKeyboardEventsTestParams{VK_LEFT, KeyCode::Left},
            WindowsKeyboardEventsTestParams{VK_UP, KeyCode::Up},
            WindowsKeyboardEventsTestParams{VK_RIGHT, KeyCode::Right},
            WindowsKeyboardEventsTestParams{VK_DOWN, KeyCode::Down},
            WindowsKeyboardEventsTestParams{VK_SELECT, KeyCode::INVALID},
            WindowsKeyboardEventsTestParams{VK_PRINT, KeyCode::PrintScreen},
            WindowsKeyboardEventsTestParams{VK_EXECUTE, KeyCode::INVALID},
            WindowsKeyboardEventsTestParams{VK_SNAPSHOT, KeyCode::PrintScreen},
            WindowsKeyboardEventsTestParams{VK_INSERT, KeyCode::Insert},
            WindowsKeyboardEventsTestParams{VK_DELETE, KeyCode::Delete},
            WindowsKeyboardEventsTestParams{VK_HELP, KeyCode::INVALID},
            WindowsKeyboardEventsTestParams{0x30, KeyCode::Alpha0},
            WindowsKeyboardEventsTestParams{0x31, KeyCode::Alpha1},
            WindowsKeyboardEventsTestParams{0x32, KeyCode::Alpha2},
            WindowsKeyboardEventsTestParams{0x33, KeyCode::Alpha3},
            WindowsKeyboardEventsTestParams{0x34, KeyCode::Alpha4},
            WindowsKeyboardEventsTestParams{0x35, KeyCode::Alpha5},
            WindowsKeyboardEventsTestParams{0x36, KeyCode::Alpha6},
            WindowsKeyboardEventsTestParams{0x37, KeyCode::Alpha7},
            WindowsKeyboardEventsTestParams{0x38, KeyCode::Alpha8},
            WindowsKeyboardEventsTestParams{0x39, KeyCode::Alpha9},
            WindowsKeyboardEventsTestParams{VK_LWIN, KeyCode::LeftSuper},
            WindowsKeyboardEventsTestParams{VK_RWIN, KeyCode::RightSuper},
            WindowsKeyboardEventsTestParams{VK_APPS, KeyCode::INVALID},
            WindowsKeyboardEventsTestParams{VK_SLEEP, KeyCode::INVALID},
            WindowsKeyboardEventsTestParams{VK_NUMPAD0, KeyCode::Num0},
            WindowsKeyboardEventsTestParams{VK_NUMPAD1, KeyCode::Num1},
            WindowsKeyboardEventsTestParams{VK_NUMPAD2, KeyCode::Num2},
            WindowsKeyboardEventsTestParams{VK_NUMPAD3, KeyCode::Num3},
            WindowsKeyboardEventsTestParams{VK_NUMPAD4, KeyCode::Num4},
            WindowsKeyboardEventsTestParams{VK_NUMPAD5, KeyCode::Num5},
            WindowsKeyboardEventsTestParams{VK_NUMPAD6, KeyCode::Num6},
            WindowsKeyboardEventsTestParams{VK_NUMPAD7, KeyCode::Num7},
            WindowsKeyboardEventsTestParams{VK_NUMPAD8, KeyCode::Num8},
            WindowsKeyboardEventsTestParams{VK_NUMPAD9, KeyCode::Num9},
            WindowsKeyboardEventsTestParams{VK_MULTIPLY, KeyCode::NumMultiply},
            WindowsKeyboardEventsTestParams{VK_ADD, KeyCode::NumAdd},
            WindowsKeyboardEventsTestParams{VK_SEPARATOR, KeyCode::NumEnter},
            WindowsKeyboardEventsTestParams{VK_SUBTRACT, KeyCode::NumSubtract},
            WindowsKeyboardEventsTestParams{VK_DECIMAL, KeyCode::NumDecimal},
            WindowsKeyboardEventsTestParams{VK_DIVIDE, KeyCode::NumDivide},
            WindowsKeyboardEventsTestParams{VK_F1, KeyCode::F1},
            WindowsKeyboardEventsTestParams{VK_F2, KeyCode::F2},
            WindowsKeyboardEventsTestParams{VK_F3, KeyCode::F3},
            WindowsKeyboardEventsTestParams{VK_F4, KeyCode::F4},
            WindowsKeyboardEventsTestParams{VK_F5, KeyCode::F5},
            WindowsKeyboardEventsTestParams{VK_F6, KeyCode::F6},
            WindowsKeyboardEventsTestParams{VK_F7, KeyCode::F7},
            WindowsKeyboardEventsTestParams{VK_F8, KeyCode::F8},
            WindowsKeyboardEventsTestParams{VK_F9, KeyCode::F9},
            WindowsKeyboardEventsTestParams{VK_F10, KeyCode::F10},
            WindowsKeyboardEventsTestParams{VK_F11, KeyCode::F11},
            WindowsKeyboardEventsTestParams{VK_F12, KeyCode::F12},
            WindowsKeyboardEventsTestParams{VK_NUMLOCK, KeyCode::NumLock},
            WindowsKeyboardEventsTestParams{VK_SCROLL, KeyCode::ScrollLock},
            WindowsKeyboardEventsTestParams{VK_LSHIFT, KeyCode::LeftShift},
            WindowsKeyboardEventsTestParams{VK_RSHIFT, KeyCode::RightShift},
            WindowsKeyboardEventsTestParams{VK_LCONTROL, KeyCode::LeftControl},
            WindowsKeyboardEventsTestParams{VK_RCONTROL, KeyCode::RightControl},
            WindowsKeyboardEventsTestParams{VK_LMENU, KeyCode::LeftAlt},
            WindowsKeyboardEventsTestParams{VK_RMENU, KeyCode::RightAlt},
            WindowsKeyboardEventsTestParams{VK_VOLUME_MUTE, KeyCode::VolumeMute},
            WindowsKeyboardEventsTestParams{VK_VOLUME_DOWN, KeyCode::VolumeDown},
            WindowsKeyboardEventsTestParams{VK_VOLUME_UP, KeyCode::VolumeUp},
            WindowsKeyboardEventsTestParams{VK_MEDIA_NEXT_TRACK, KeyCode::MediaNextTrack},
            WindowsKeyboardEventsTestParams{VK_MEDIA_PREV_TRACK, KeyCode::MediaPreviousTrack},
            WindowsKeyboardEventsTestParams{VK_MEDIA_STOP, KeyCode::MediaStop},
            WindowsKeyboardEventsTestParams{VK_MEDIA_PLAY_PAUSE, KeyCode::MediaPlayAndPause},
            WindowsKeyboardEventsTestParams{VK_OEM_1, KeyCode::Semicolon},
            WindowsKeyboardEventsTestParams{VK_OEM_PLUS, KeyCode::EqualAdd},
            WindowsKeyboardEventsTestParams{VK_OEM_COMMA, KeyCode::Comma},
            WindowsKeyboardEventsTestParams{VK_OEM_MINUS, KeyCode::Minus},
            WindowsKeyboardEventsTestParams{VK_OEM_PERIOD, KeyCode::Period},
            WindowsKeyboardEventsTestParams{VK_OEM_2, KeyCode::Slash},
            WindowsKeyboardEventsTestParams{VK_OEM_3, KeyCode::Tilde},
            WindowsKeyboardEventsTestParams{VK_OEM_4, KeyCode::LeftBracket},
            WindowsKeyboardEventsTestParams{VK_OEM_5, KeyCode::Backslash},
            WindowsKeyboardEventsTestParams{VK_OEM_6, KeyCode::RightBracket},
            WindowsKeyboardEventsTestParams{VK_OEM_7, KeyCode::Quote},
            WindowsKeyboardEventsTestParams{VK_OEM_8, KeyCode::OEMSpecific}
        )
    );

    TEST_P(KeyboardEventsTest, ProcessInputWillProperlyHandleAndDispatchKeyPressedMessages)
    {
        const WindowsKeyboardEventsTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;
        const auto expectedButtonCode = testParams.expectedKeyCode;

        KeyboardEventHandler expectedHandler = [&](const KeyboardEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetKey());
        };

        auto sut = GetWindow();
        sut->SetKeyboardEventsHandler(expectedHandler);

        // Send WM_KEYDOWN WinAPI message
        PostMessage(sut->GetNativeHandle(), WM_KEYDOWN, testParams.virtualKeyCode, NULL);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(KeyboardEventsTest, ProcessInputWillProperlyHandleAndDispatchKeyReleasedMessages)
    {
        const WindowsKeyboardEventsTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_RELEASED;
        const auto expectedButtonCode = testParams.expectedKeyCode;

        KeyboardEventHandler expectedHandler = [&](const KeyboardEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetKey());
        };

        auto sut = GetWindow();
        sut->SetKeyboardEventsHandler(expectedHandler);

        // Send WM_KEYUP WinAPI message
        PostMessage(sut->GetNativeHandle(), WM_KEYUP, testParams.virtualKeyCode, NULL);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(KeyboardEventsTest, ProcessInputWillProperlyHandleAndDispatchKeyHeldDownMessages)
    {
        const WindowsKeyboardEventsTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_HELD_DOWN;
        const auto expectedButtonCode = testParams.expectedKeyCode;

        KeyboardEventHandler expectedHandler = [&](const KeyboardEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetKey());
        };

        auto sut = GetWindow();
        sut->SetKeyboardEventsHandler(expectedHandler);

        // Send WM_KEYDOWN WinAPI message
        LPARAM keyDownRepeated = (1 << 30); // Bit 30 set to 1 defines that the key was previously pressed
        PostMessage(sut->GetNativeHandle(), WM_KEYDOWN, testParams.virtualKeyCode, keyDownRepeated);

        sut->ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
} // namespace be::tests::integration
#endif
