#ifdef BE_PLATFORM_WINDOWS
    #include "BeastEngine/Core/Windows/Win32/Win32Window.h"
    #include "BeastEngine/Core/Assertions.h"
    #include "BeastEngine/Core/Events/Events.h"

    #include <Common/Exceptions.h>
    #include <unordered_map>

namespace be::internals
{
    #ifndef BE_WINAPI_CALL
        #define BE_WINAPI_CALL(call)                                                                                    \
            {                                                                                                           \
                SetLastError(0);                                                                                        \
                const auto winapiCallResult = call;                                                                     \
                const auto lastError = GetLastError();                                                                  \
                if (winapiCallResult == 0 && lastError != 0)                                                            \
                {                                                                                                       \
                    const std::string errorMessage =                                                                    \
                        "An error occurred when calling the WinApi function. Error code: " + std::to_string(lastError); \
                    CT_THROW(errorMessage);                                                                             \
                }                                                                                                       \
            }
    #endif

    // Mappings between Win32 key codes and Engine's KeyCodes
    // It's an unordered_map instead of the constexpr map, because of its size.
    // Unordered_map is better for a bigger data set frequent lookups.
    // Measure performance if needed.
    static const std::unordered_map<WPARAM, KeyCode> KEY_CODES_MAP{
        {0x41, KeyCode::A},
        {0x42, KeyCode::B},
        {0x43, KeyCode::C},
        {0x44, KeyCode::D},
        {0x45, KeyCode::E},
        {0x46, KeyCode::F},
        {0x47, KeyCode::G},
        {0x48, KeyCode::H},
        {0x49, KeyCode::I},
        {0x4A, KeyCode::J},
        {0x4B, KeyCode::K},
        {0x4C, KeyCode::L},
        {0x4D, KeyCode::M},
        {0x4E, KeyCode::N},
        {0x4F, KeyCode::O},
        {0x50, KeyCode::P},
        {0x51, KeyCode::Q},
        {0x52, KeyCode::R},
        {0x53, KeyCode::S},
        {0x54, KeyCode::T},
        {0x55, KeyCode::U},
        {0x56, KeyCode::V},
        {0x57, KeyCode::X},
        {0x58, KeyCode::Y},
        {0x59, KeyCode::Z},
        {VK_BACK, KeyCode::Backspace},
        {VK_TAB, KeyCode::Tab},
        {VK_CLEAR, KeyCode::Delete},
        {VK_RETURN, KeyCode::Enter},
        {VK_SHIFT, KeyCode::LeftShift},
        {VK_CONTROL, KeyCode::LeftControl},
        {VK_MENU, KeyCode::LeftAlt},
        {VK_PAUSE, KeyCode::Pause},
        {VK_CAPITAL, KeyCode::CapsLock},
        {VK_ESCAPE, KeyCode::Escape},
        {VK_SPACE, KeyCode::Space},
        {VK_PRIOR, KeyCode::PageUp},
        {VK_NEXT, KeyCode::PageDown},
        {VK_END, KeyCode::End},
        {VK_HOME, KeyCode::Home},
        {VK_LEFT, KeyCode::Left},
        {VK_UP, KeyCode::Up},
        {VK_RIGHT, KeyCode::Right},
        {VK_DOWN, KeyCode::Down},
        {VK_SELECT, KeyCode::INVALID},
        {VK_PRINT, KeyCode::PrintScreen},
        {VK_EXECUTE, KeyCode::INVALID},
        {VK_SNAPSHOT, KeyCode::PrintScreen},
        {VK_INSERT, KeyCode::Insert},
        {VK_DELETE, KeyCode::Delete},
        {VK_HELP, KeyCode::INVALID},
        {0x30, KeyCode::Alpha0},
        {0x31, KeyCode::Alpha1},
        {0x32, KeyCode::Alpha2},
        {0x33, KeyCode::Alpha3},
        {0x34, KeyCode::Alpha4},
        {0x35, KeyCode::Alpha5},
        {0x36, KeyCode::Alpha6},
        {0x37, KeyCode::Alpha7},
        {0x38, KeyCode::Alpha8},
        {0x39, KeyCode::Alpha9},
        {VK_LWIN, KeyCode::LeftSuper},
        {VK_RWIN, KeyCode::RightSuper},
        {VK_APPS, KeyCode::INVALID},
        {VK_SLEEP, KeyCode::INVALID},
        {VK_NUMPAD0, KeyCode::Num0},
        {VK_NUMPAD1, KeyCode::Num1},
        {VK_NUMPAD2, KeyCode::Num2},
        {VK_NUMPAD3, KeyCode::Num3},
        {VK_NUMPAD4, KeyCode::Num4},
        {VK_NUMPAD5, KeyCode::Num5},
        {VK_NUMPAD6, KeyCode::Num6},
        {VK_NUMPAD7, KeyCode::Num7},
        {VK_NUMPAD8, KeyCode::Num8},
        {VK_NUMPAD9, KeyCode::Num9},
        {VK_MULTIPLY, KeyCode::NumMultiply},
        {VK_ADD, KeyCode::NumAdd},
        {VK_SEPARATOR, KeyCode::NumEnter},
        {VK_SUBTRACT, KeyCode::NumSubtract},
        {VK_DECIMAL, KeyCode::NumDecimal},
        {VK_DIVIDE, KeyCode::NumDivide},
        {VK_F1, KeyCode::F1},
        {VK_F2, KeyCode::F2},
        {VK_F3, KeyCode::F3},
        {VK_F4, KeyCode::F4},
        {VK_F5, KeyCode::F5},
        {VK_F6, KeyCode::F6},
        {VK_F7, KeyCode::F7},
        {VK_F8, KeyCode::F8},
        {VK_F9, KeyCode::F9},
        {VK_F10, KeyCode::F10},
        {VK_F11, KeyCode::F11},
        {VK_F12, KeyCode::F12},
        {VK_NUMLOCK, KeyCode::NumLock},
        {VK_SCROLL, KeyCode::ScrollLock},
        {VK_LSHIFT, KeyCode::LeftShift},
        {VK_RSHIFT, KeyCode::RightShift},
        {VK_LCONTROL, KeyCode::LeftControl},
        {VK_RCONTROL, KeyCode::RightControl},
        {VK_LMENU, KeyCode::LeftAlt},
        {VK_RMENU, KeyCode::RightAlt},
        {VK_VOLUME_MUTE, KeyCode::VolumeMute},
        {VK_VOLUME_DOWN, KeyCode::VolumeDown},
        {VK_VOLUME_UP, KeyCode::VolumeUp},
        {VK_MEDIA_NEXT_TRACK, KeyCode::MediaNextTrack},
        {VK_MEDIA_PREV_TRACK, KeyCode::MediaPreviousTrack},
        {VK_MEDIA_STOP, KeyCode::MediaStop},
        {VK_MEDIA_PLAY_PAUSE, KeyCode::MediaPlayAndPause},
        {VK_OEM_1, KeyCode::Semicolon},
        {VK_OEM_PLUS, KeyCode::EqualAdd},
        {VK_OEM_COMMA, KeyCode::Comma},
        {VK_OEM_MINUS, KeyCode::Minus},
        {VK_OEM_PERIOD, KeyCode::Period},
        {VK_OEM_2, KeyCode::Slash},
        {VK_OEM_3, KeyCode::Tilde},
        {VK_OEM_4, KeyCode::LeftBracket},
        {VK_OEM_5, KeyCode::Backslash},
        {VK_OEM_6, KeyCode::RightBracket},
        {VK_OEM_7, KeyCode::Quote},
        {VK_OEM_8, KeyCode::OEMSpecific},
    };

    // Mappings between Engine's WindowStyles and Win32 window styles.
    // Used to choose valid Win32 style based on the provided Engine's WindowStyle.
    using WindowFlagsMap = ConstexprMap<WindowStyle, uint32, 3>;
    static constexpr WindowFlagsMap WINDOW_STYLES_MAP{
        WindowFlagsMap::StorageType{
            {
                {WindowStyle::WINDOW_DEFUALT, WS_OVERLAPPEDWINDOW},
                {WindowStyle::WINDOW_BORDERLESS, WS_OVERLAPPED},
                {WindowStyle::WINDOW_FULLSCREEN, WS_OVERLAPPEDWINDOW},
            },
        },
    };

    // Mappings between Win32 button codes and Engine's mouse button codes.
    // Used to lookup Engine's code based on the Win32 code from the event.
    using MouseButtonsMap = ConstexprMap<uint64, MouseButtonCode, 8>;
    static constexpr MouseButtonsMap MOUSE_BUTTONS_CODES_MAP{
        MouseButtonsMap::StorageType{
            {
                {WM_LBUTTONDOWN, MouseButtonCode::BUTTON1},
                {WM_LBUTTONUP, MouseButtonCode::BUTTON1},
                {WM_RBUTTONDOWN, MouseButtonCode::BUTTON2},
                {WM_RBUTTONUP, MouseButtonCode::BUTTON2},
                {WM_MBUTTONDOWN, MouseButtonCode::BUTTON3},
                {WM_MBUTTONUP, MouseButtonCode::BUTTON3},
                {XBUTTON1, MouseButtonCode::BUTTON4},
                {XBUTTON2, MouseButtonCode::BUTTON5},
            },
        },
    };

    // Mappings between Engine's WindowStyles and Win32 display parameters.
    // Used to choose Win32 display style based on the provided Engine's WindowStyle.
    using WindowDisplayParamsMap = ConstexprMap<WindowStyle, uint32, 3>;
    static constexpr WindowDisplayParamsMap DISPLAY_PARAMS_MAP{
        WindowDisplayParamsMap::StorageType{
            {
                {WindowStyle::WINDOW_DEFUALT, SW_SHOW},
                {WindowStyle::WINDOW_FULLSCREEN, SW_SHOW},
                {WindowStyle::WINDOW_BORDERLESS, SW_MAXIMIZE},
            },
        },
    };

    Win32Window::Win32Window(const WindowDescriptor& windowDescriptor, const wchar_t* windowClassName)
        : WINDOW_CLASS_NAME(windowClassName), m_hInstance(windowDescriptor.handleInstance.Get()), m_descriptor(windowDescriptor)
    {
        SetUpMessageHandlers();
        WNDCLASS wc = {0};

        wc.lpfnWndProc = WindowProcSetup;
        wc.hInstance = m_hInstance;
        wc.lpszClassName = WINDOW_CLASS_NAME;
        BE_WINAPI_CALL(RegisterClass(&wc));

        const auto windowDimensions = GetWindowDimensions();
        m_hwnd = CreateWindowW(
            WINDOW_CLASS_NAME,
            ConvertWindowTitle(m_descriptor.title).c_str(),
            GetWindowStyle(m_descriptor.style),
            m_descriptor.position.x,
            m_descriptor.position.y,
            windowDimensions.x,
            windowDimensions.y,
            NULL, // Parent window
            NULL, // Menu
            wc.hInstance,
            this);

        if (m_hwnd == NULL)
        {
            CT_THROW("Could not create window. Reason: " + GetLastError());
        }

        DisplayWindow();
    }

    Win32Window::~Win32Window()
    {
        BE_ASSERT(DestroyWindow(m_hwnd));

        WNDCLASS windowClass = {0};
        if (GetClassInfo(m_hInstance, WINDOW_CLASS_NAME, &windowClass))
        {
            BE_ASSERT(UnregisterClass(WINDOW_CLASS_NAME, m_hInstance));
        }
    }

    void Win32Window::ProcessInput()
    {
        ProcessHeldDownMessages();

        MSG msg = {};
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    HWND Win32Window::GetNativeHandle() const noexcept
    {
        return m_hwnd;
    }

    std::wstring Win32Window::ConvertWindowTitle(const std::string& narrowTitle) const
    {
        const auto wcharBufferSize = MultiByteToWideChar(CP_UTF8, 0, narrowTitle.c_str(), -1, nullptr, 0);
        auto wideTitle = std::wstring(wcharBufferSize, 0);

        const auto result = MultiByteToWideChar(CP_UTF8, 0, narrowTitle.c_str(), -1, &wideTitle[0], wcharBufferSize);
        BE_ASSERT(result);

        return wideTitle;
    }

    uint32 Win32Window::GetWindowStyle(WindowStyle windowStyle) const
    {
        return WINDOW_STYLES_MAP.At(windowStyle);
    }

    IntVec2 Win32Window::GetWindowDimensions() const
    {
        if (m_descriptor.style == WindowStyle::WINDOW_FULLSCREEN)
        {
            return {GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)};
        }

        return m_descriptor.dimensions;
    }

    void Win32Window::DisplayWindow() const
    {
        ShowWindow(m_hwnd, DISPLAY_PARAMS_MAP.At(m_descriptor.style));
        if (m_descriptor.style == WindowStyle::WINDOW_FULLSCREEN)
        {
            ToggleToFullscreen();
        }
    }

    void Win32Window::ToggleToFullscreen() const
    {
        const auto dimensions = GetWindowDimensions();

        BE_WINAPI_CALL(SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP));
        SetWindowPos(m_hwnd, HWND_TOP, 0, 0, dimensions.x, dimensions.y, SWP_FRAMECHANGED);
    }

    LRESULT Win32Window::WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg != WM_NCCREATE)
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        // Get ptr to Win32Window instance from winapi window creation data
        const CREATESTRUCTW* const windowParams = reinterpret_cast<CREATESTRUCTW*>(lParam);
        const Win32Window* const owningWindow = static_cast<Win32Window*>(windowParams->lpCreateParams);

        // Store Win32Window instance in the winapi user data
        BE_WINAPI_CALL(SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(owningWindow)));

        // Set window proc to normal (non-setup) handler now that the setup is finished
        BE_WINAPI_CALL(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Win32Window::WindowProcThunk)));

        // Forward message to Win32Window instance member function responsible for handling messages
        return owningWindow->HandleWindowMessages(hWnd, uMsg, wParam, lParam);
    }

    LRESULT Win32Window::WindowProcThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        const Win32Window* const window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return window->HandleWindowMessages(hWnd, uMsg, wParam, lParam);
    }

    void Win32Window::SetUpMessageHandlers()
    {
        SetUpMouseMessagesHandlers();
        SetUpKeyboardMessagesHandlers();

        m_messageHandlers[WM_CLOSE] = [&](UINT, WPARAM, LPARAM) {
            DispatchWindowClosedEvent();
            PostQuitMessage(0);

            return 0;
        };
    }

    void Win32Window::SetUpMouseMessagesHandlers()
    {
        static constexpr const std::array<UINT, 4> buttonDownKeys = {WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_MBUTTONDOWN, WM_XBUTTONDOWN};
        for (const UINT key : buttonDownKeys)
        {
            m_messageHandlers[key] = [&](auto&&... args) { return HandleMouseButtonDownMessages(args...); };
        }

        static constexpr const std::array<UINT, 5> buttonUpKeys = {WM_LBUTTONUP, WM_LBUTTONUP, WM_RBUTTONUP, WM_MBUTTONUP, WM_XBUTTONUP};
        for (const UINT key : buttonUpKeys)
        {
            m_messageHandlers[key] = [&](auto&&... args) { return HandleMouseButtonUpMessages(args...); };
        }

        m_messageHandlers[WM_MOUSEMOVE] = [&](UINT, WPARAM, LPARAM lParam) {
            DispatchEvent(MouseEvent::Moved(GetMouseCoordinates(lParam)));
            return 0;
        };

        m_messageHandlers[WM_MOUSEWHEEL] = [&](UINT, WPARAM wParam, LPARAM lParam) {
            DispatchEvent(MouseEvent::Scrolled(GET_WHEEL_DELTA_WPARAM(wParam), GetMouseCoordinates(lParam)));
            return 0;
        };
    }

    void Win32Window::SetUpKeyboardMessagesHandlers()
    {
        m_messageHandlers[WM_KEYDOWN] = [&](auto&&... args) {
            return HandleKeyDownMessages(args...);
        };

        m_messageHandlers[WM_KEYUP] = [&](UINT, WPARAM wParam, LPARAM) {
            DispatchEvent(KeyboardEvent::KeyReleased(KEY_CODES_MAP.at(wParam)));
            return 0;
        };
    }

    LRESULT Win32Window::HandleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
    {
        // Invoke handler defined for this message, or the default one if no defined
        try
        {
            return m_messageHandlers.at(uMsg)(uMsg, wParam, lParam);
        }
        catch (const std::out_of_range&)
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    LRESULT Win32Window::HandleMouseButtonDownMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) const
    {
        // Capture cursor so even if it goes out of window's border, messages are still received
        SetCapture(m_hwnd);

        const auto buttonCode = (uMsg == WM_XBUTTONDOWN) ? GET_XBUTTON_WPARAM(wParam) : uMsg;
        DispatchEvent(MouseEvent::ButtonPressed(MOUSE_BUTTONS_CODES_MAP.At(buttonCode), GetMouseCoordinates(lParam)));

        // Message successfully handled
        return 0;
    }

    LRESULT Win32Window::HandleMouseButtonUpMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) const
    {
        // Release cursor
        ReleaseCapture();

        const auto buttonCode = (uMsg == WM_XBUTTONUP) ? GET_XBUTTON_WPARAM(wParam) : uMsg;
        DispatchEvent(MouseEvent::ButtonReleased(MOUSE_BUTTONS_CODES_MAP.At(buttonCode), GetMouseCoordinates(lParam)));

        // Message successfully handled
        return 0;
    }

    LRESULT Win32Window::HandleKeyDownMessages(UINT, WPARAM wParam, LPARAM lParam) const
    {
        const auto keyCode = KEY_CODES_MAP.at(wParam);
        if (IsKeyHeldDown(lParam))
        {
            DispatchEvent(KeyboardEvent::KeyHeldDown(keyCode));
        }
        else
        {
            DispatchEvent(KeyboardEvent::KeyPressed(keyCode));
        }

        // Message successfully handled
        return 0;
    }

    void Win32Window::ProcessHeldDownMessages() const
    {
        // All of those events can occurr at the same time
        // That's why we check for every one individually.

        if (IsKeyPressed(VK_LBUTTON))
        {
            DispatchEvent(MouseEvent::ButtonHeldDown(MouseButtonCode::BUTTON_LEFT));
        }

        if (IsKeyPressed(VK_MBUTTON))
        {
            DispatchEvent(MouseEvent::ButtonHeldDown(MouseButtonCode::BUTTON_MIDDLE));
        }

        if (IsKeyPressed(VK_RBUTTON))
        {
            DispatchEvent(MouseEvent::ButtonHeldDown(MouseButtonCode::BUTTON_RIGHT));
        }

        if (IsKeyPressed(VK_XBUTTON1))
        {
            DispatchEvent(MouseEvent::ButtonHeldDown(MouseButtonCode::BUTTON4));
        }

        if (IsKeyPressed(VK_XBUTTON2))
        {
            DispatchEvent(MouseEvent::ButtonHeldDown(MouseButtonCode::BUTTON5));
        }
    }
} // namespace be::internals
#endif
