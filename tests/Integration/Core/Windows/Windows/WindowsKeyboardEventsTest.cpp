#include "WindowsKeyboardEventsTest.h"

#if LAB_CAN_BUILD_WINAPI_TESTS
    #include <BeastEngine/Core/Windows/Windows/WindowsWindow.h>
    #include <BeastEngine/Core/Events/Events.h>

namespace be::tests::integration
{
    INSTANTIATE_TEST_SUITE_P(
        WindowsWindowTest_KeyboardMessagesTest,
        KeyboardMessagesTest,
        testing::Values(
            KeyboardMessageTestParams{0x41, KeyCode::A},
            KeyboardMessageTestParams{0x42, KeyCode::B},
            KeyboardMessageTestParams{0x43, KeyCode::C},
            KeyboardMessageTestParams{0x44, KeyCode::D},
            KeyboardMessageTestParams{0x45, KeyCode::E},
            KeyboardMessageTestParams{0x46, KeyCode::F},
            KeyboardMessageTestParams{0x47, KeyCode::G},
            KeyboardMessageTestParams{0x48, KeyCode::H},
            KeyboardMessageTestParams{0x49, KeyCode::I},
            KeyboardMessageTestParams{0x4A, KeyCode::J},
            KeyboardMessageTestParams{0x4B, KeyCode::K},
            KeyboardMessageTestParams{0x4C, KeyCode::L},
            KeyboardMessageTestParams{0x4D, KeyCode::M},
            KeyboardMessageTestParams{0x4E, KeyCode::N},
            KeyboardMessageTestParams{0x4F, KeyCode::O},
            KeyboardMessageTestParams{0x50, KeyCode::P},
            KeyboardMessageTestParams{0x51, KeyCode::Q},
            KeyboardMessageTestParams{0x52, KeyCode::R},
            KeyboardMessageTestParams{0x53, KeyCode::S},
            KeyboardMessageTestParams{0x54, KeyCode::T},
            KeyboardMessageTestParams{0x55, KeyCode::U},
            KeyboardMessageTestParams{0x56, KeyCode::V},
            KeyboardMessageTestParams{0x57, KeyCode::X},
            KeyboardMessageTestParams{0x58, KeyCode::Y},
            KeyboardMessageTestParams{0x59, KeyCode::Z},
            KeyboardMessageTestParams{VK_BACK, KeyCode::Backspace},
            KeyboardMessageTestParams{VK_TAB, KeyCode::Tab},
            KeyboardMessageTestParams{VK_CLEAR, KeyCode::Delete},
            KeyboardMessageTestParams{VK_RETURN, KeyCode::Enter},
            KeyboardMessageTestParams{VK_SHIFT, KeyCode::LeftShift},
            KeyboardMessageTestParams{VK_CONTROL, KeyCode::LeftControl},
            KeyboardMessageTestParams{VK_MENU, KeyCode::LeftAlt},
            KeyboardMessageTestParams{VK_PAUSE, KeyCode::Pause},
            KeyboardMessageTestParams{VK_CAPITAL, KeyCode::CapsLock},
            KeyboardMessageTestParams{VK_ESCAPE, KeyCode::Escape},
            KeyboardMessageTestParams{VK_SPACE, KeyCode::Space},
            KeyboardMessageTestParams{VK_PRIOR, KeyCode::PageUp},
            KeyboardMessageTestParams{VK_NEXT, KeyCode::PageDown},
            KeyboardMessageTestParams{VK_END, KeyCode::End},
            KeyboardMessageTestParams{VK_HOME, KeyCode::Home},
            KeyboardMessageTestParams{VK_LEFT, KeyCode::Left},
            KeyboardMessageTestParams{VK_UP, KeyCode::Up},
            KeyboardMessageTestParams{VK_RIGHT, KeyCode::Right},
            KeyboardMessageTestParams{VK_DOWN, KeyCode::Down},
            KeyboardMessageTestParams{VK_SELECT, KeyCode::INVALID},
            KeyboardMessageTestParams{VK_PRINT, KeyCode::PrintScreen},
            KeyboardMessageTestParams{VK_EXECUTE, KeyCode::INVALID},
            KeyboardMessageTestParams{VK_SNAPSHOT, KeyCode::PrintScreen},
            KeyboardMessageTestParams{VK_INSERT, KeyCode::Insert},
            KeyboardMessageTestParams{VK_DELETE, KeyCode::Delete},
            KeyboardMessageTestParams{VK_HELP, KeyCode::INVALID},
            KeyboardMessageTestParams{0x30, KeyCode::Alpha0},
            KeyboardMessageTestParams{0x31, KeyCode::Alpha1},
            KeyboardMessageTestParams{0x32, KeyCode::Alpha2},
            KeyboardMessageTestParams{0x33, KeyCode::Alpha3},
            KeyboardMessageTestParams{0x34, KeyCode::Alpha4},
            KeyboardMessageTestParams{0x35, KeyCode::Alpha5},
            KeyboardMessageTestParams{0x36, KeyCode::Alpha6},
            KeyboardMessageTestParams{0x37, KeyCode::Alpha7},
            KeyboardMessageTestParams{0x38, KeyCode::Alpha8},
            KeyboardMessageTestParams{0x39, KeyCode::Alpha9},
            KeyboardMessageTestParams{VK_LWIN, KeyCode::LeftSuper},
            KeyboardMessageTestParams{VK_RWIN, KeyCode::RightSuper},
            KeyboardMessageTestParams{VK_APPS, KeyCode::INVALID},
            KeyboardMessageTestParams{VK_SLEEP, KeyCode::INVALID},
            KeyboardMessageTestParams{VK_NUMPAD0, KeyCode::Num0},
            KeyboardMessageTestParams{VK_NUMPAD1, KeyCode::Num1},
            KeyboardMessageTestParams{VK_NUMPAD2, KeyCode::Num2},
            KeyboardMessageTestParams{VK_NUMPAD3, KeyCode::Num3},
            KeyboardMessageTestParams{VK_NUMPAD4, KeyCode::Num4},
            KeyboardMessageTestParams{VK_NUMPAD5, KeyCode::Num5},
            KeyboardMessageTestParams{VK_NUMPAD6, KeyCode::Num6},
            KeyboardMessageTestParams{VK_NUMPAD7, KeyCode::Num7},
            KeyboardMessageTestParams{VK_NUMPAD8, KeyCode::Num8},
            KeyboardMessageTestParams{VK_NUMPAD9, KeyCode::Num9},
            KeyboardMessageTestParams{VK_MULTIPLY, KeyCode::NumMultiply},
            KeyboardMessageTestParams{VK_ADD, KeyCode::NumAdd},
            KeyboardMessageTestParams{VK_SEPARATOR, KeyCode::NumEnter},
            KeyboardMessageTestParams{VK_SUBTRACT, KeyCode::NumSubtract},
            KeyboardMessageTestParams{VK_DECIMAL, KeyCode::NumDecimal},
            KeyboardMessageTestParams{VK_DIVIDE, KeyCode::NumDivide},
            KeyboardMessageTestParams{VK_F1, KeyCode::F1},
            KeyboardMessageTestParams{VK_F2, KeyCode::F2},
            KeyboardMessageTestParams{VK_F3, KeyCode::F3},
            KeyboardMessageTestParams{VK_F4, KeyCode::F4},
            KeyboardMessageTestParams{VK_F5, KeyCode::F5},
            KeyboardMessageTestParams{VK_F6, KeyCode::F6},
            KeyboardMessageTestParams{VK_F7, KeyCode::F7},
            KeyboardMessageTestParams{VK_F8, KeyCode::F8},
            KeyboardMessageTestParams{VK_F9, KeyCode::F9},
            KeyboardMessageTestParams{VK_F10, KeyCode::F10},
            KeyboardMessageTestParams{VK_F11, KeyCode::F11},
            KeyboardMessageTestParams{VK_F12, KeyCode::F12},
            KeyboardMessageTestParams{VK_NUMLOCK, KeyCode::NumLock},
            KeyboardMessageTestParams{VK_SCROLL, KeyCode::ScrollLock},
            KeyboardMessageTestParams{VK_LSHIFT, KeyCode::LeftShift},
            KeyboardMessageTestParams{VK_RSHIFT, KeyCode::RightShift},
            KeyboardMessageTestParams{VK_LCONTROL, KeyCode::LeftControl},
            KeyboardMessageTestParams{VK_RCONTROL, KeyCode::RightControl},
            KeyboardMessageTestParams{VK_LMENU, KeyCode::LeftAlt},
            KeyboardMessageTestParams{VK_RMENU, KeyCode::RightAlt},
            KeyboardMessageTestParams{VK_VOLUME_MUTE, KeyCode::VolumeMute},
            KeyboardMessageTestParams{VK_VOLUME_DOWN, KeyCode::VolumeDown},
            KeyboardMessageTestParams{VK_VOLUME_UP, KeyCode::VolumeUp},
            KeyboardMessageTestParams{VK_MEDIA_NEXT_TRACK, KeyCode::MediaNextTrack},
            KeyboardMessageTestParams{VK_MEDIA_PREV_TRACK, KeyCode::MediaPreviousTrack},
            KeyboardMessageTestParams{VK_MEDIA_STOP, KeyCode::MediaStop},
            KeyboardMessageTestParams{VK_MEDIA_PLAY_PAUSE, KeyCode::MediaPlayAndPause},
            KeyboardMessageTestParams{VK_OEM_1, KeyCode::Semicolon},
            KeyboardMessageTestParams{VK_OEM_PLUS, KeyCode::EqualAdd},
            KeyboardMessageTestParams{VK_OEM_COMMA, KeyCode::Comma},
            KeyboardMessageTestParams{VK_OEM_MINUS, KeyCode::Minus},
            KeyboardMessageTestParams{VK_OEM_PERIOD, KeyCode::Period},
            KeyboardMessageTestParams{VK_OEM_2, KeyCode::Slash},
            KeyboardMessageTestParams{VK_OEM_3, KeyCode::Tilde},
            KeyboardMessageTestParams{VK_OEM_4, KeyCode::LeftBracket},
            KeyboardMessageTestParams{VK_OEM_5, KeyCode::Backslash},
            KeyboardMessageTestParams{VK_OEM_6, KeyCode::RightBracket},
            KeyboardMessageTestParams{VK_OEM_7, KeyCode::Quote},
            KeyboardMessageTestParams{VK_OEM_8, KeyCode::OEMSpecific}
        )
    );

    TEST_P(KeyboardMessagesTest, ProcessInputWillProperlyHandleAndDispatchKeyPressedMessages)
    {
        const KeyboardMessageTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;
        const auto expectedButtonCode = testParams.expectedKeyCode;

        KeyboardEventHandler expectedHandler = [&](const KeyboardEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetKey());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetKeyboardEventsHandler(expectedHandler);

        // Send WM_KEYDOWN WinAPI message
        PostMessage(sut.GetNativeHandle(), WM_KEYDOWN, testParams.virtualKeyCode, NULL);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(KeyboardMessagesTest, ProcessInputWillProperlyHandleAndDispatchKeyReleasedMessages)
    {
        const KeyboardMessageTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_RELEASED;
        const auto expectedButtonCode = testParams.expectedKeyCode;

        KeyboardEventHandler expectedHandler = [&](const KeyboardEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetKey());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetKeyboardEventsHandler(expectedHandler);

        // Send WM_KEYUP WinAPI message
        PostMessage(sut.GetNativeHandle(), WM_KEYUP, testParams.virtualKeyCode, NULL);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }

    TEST_P(KeyboardMessagesTest, ProcessInputWillProperlyHandleAndDispatchKeyHeldDownMessages)
    {
        const KeyboardMessageTestParams testParams = GetParam();

        bool wasHandlerCalled = false;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_HELD_DOWN;
        const auto expectedButtonCode = testParams.expectedKeyCode;

        KeyboardEventHandler expectedHandler = [&](const KeyboardEvent& event) {
            wasHandlerCalled = true;

            ASSERT_EQ(expectedEventType, event.GetType());
            ASSERT_EQ(expectedButtonCode, event.GetKey());
        };

        // Make window invisible to avoid accidental mouse movement event triggering
        WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
        windowDescriptor.dimensions = {0, 0};

        auto sut = be::internals::WindowsWindow(windowDescriptor);
        sut.SetKeyboardEventsHandler(expectedHandler);

        // Send WM_KEYDOWN WinAPI message
        LPARAM keyDownRepeated = (1 << 30); // Bit 30 set to 1 defines that the key was previously pressed
        PostMessage(sut.GetNativeHandle(), WM_KEYDOWN, testParams.virtualKeyCode, keyDownRepeated);

        sut.ProcessInput();
        ASSERT_TRUE(wasHandlerCalled);
    }
} // namespace be::tests::integration
#endif
