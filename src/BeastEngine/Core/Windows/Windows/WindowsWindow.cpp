#ifdef BE_PLATFORM_WINDOWS
    #include "BeastEngine/Core/Windows/Windows/WindowsWindow.h"
    #include "BeastEngine/Core/Assertions.h"

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
                    throw std::runtime_error(errorMessage);                                                             \
                }                                                                                                       \
            }
    #endif

    static const std::unordered_map<WPARAM, KeyCode> KEY_CODES_MAP
    {
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

    WindowsWindow::WindowsWindow(const WindowDescriptor& windowDescriptor)
        : m_hInstance(windowDescriptor.handleInstance.Get()), m_descriptor(windowDescriptor)
    {
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
            throw std::runtime_error("Could not create window. Reason: " + GetLastError());
        }

        DisplayWindow();
    }

    WindowsWindow::~WindowsWindow()
    {
        DestroyWindow(m_hwnd);
        UnregisterClass(WINDOW_CLASS_NAME, m_hInstance);
    }

    void WindowsWindow::ProcessInput()
    {
        ProcessHeldDownMessages();

        MSG msg = {};
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    HWND WindowsWindow::GetNativeHandle() const noexcept
    {
        return m_hwnd;
    }

    std::wstring WindowsWindow::ConvertWindowTitle(const std::string& narrowTitle) const
    {
        const auto wcharBufferSize = MultiByteToWideChar(CP_UTF8, 0, narrowTitle.c_str(), -1, nullptr, 0);
        auto wideTitle = std::wstring(wcharBufferSize, 0);

        const auto result = MultiByteToWideChar(CP_UTF8, 0, narrowTitle.c_str(), -1, &wideTitle[0], wcharBufferSize);
        BE_ASSERT(result);

        return wideTitle;
    }

    uint32 WindowsWindow::GetWindowStyle(WindowStyle windowStyle) const
    {
        return m_windowStylesMap.At(windowStyle);
    }

    IntVec2 WindowsWindow::GetWindowDimensions() const
    {
        if (m_descriptor.style == WindowStyle::WINDOW_FULLSCREEN)
        {
            return {GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)};
        }

        return m_descriptor.dimensions;
    }

    void WindowsWindow::DisplayWindow() const
    {
        ShowWindow(m_hwnd, m_displayParamsMap.At(m_descriptor.style));
        if (m_descriptor.style == WindowStyle::WINDOW_FULLSCREEN)
        {
            ToggleToFullscreen();
        }
    }

    void WindowsWindow::ToggleToFullscreen() const
    {
        const auto dimensions = GetWindowDimensions();

        BE_WINAPI_CALL(SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP));
        SetWindowPos(m_hwnd, HWND_TOP, 0, 0, dimensions.x, dimensions.y, SWP_FRAMECHANGED);
    }

    LRESULT WindowsWindow::WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg != WM_NCCREATE)
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        // Get ptr to WindowsWindow instance from winapi window creation data
        const CREATESTRUCTW* const windowParams = reinterpret_cast<CREATESTRUCTW*>(lParam);
        const WindowsWindow* const owningWindow = static_cast<WindowsWindow*>(windowParams->lpCreateParams);

        // Store WindowsWindow instance in the winapi user data
        BE_WINAPI_CALL(SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(owningWindow)));

        // Set window proc to normal (non-setup) handler now that setup is finished
        BE_WINAPI_CALL(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::WindowProcThunk)));

        // Forward message to WindowsWindow instance member function responsible for handling messages
        return owningWindow->HandleWindowMessages(hWnd, uMsg, wParam, lParam);
    }

    LRESULT WindowsWindow::WindowProcThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        const WindowsWindow* const window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return window->HandleWindowMessages(hWnd, uMsg, wParam, lParam);
    }

    LRESULT WindowsWindow::HandleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
    {
        switch (uMsg)
        {
        case WM_CLOSE:
            DispatchWindowClosedEvent();
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            if (IsKeyHeldDown(lParam))
            {
                DispatchEvent(KeyHeldDownEvent(KEY_CODES_MAP.at(wParam)));
                break;
            }

            DispatchEvent(KeyPressedEvent(KEY_CODES_MAP.at(wParam)));
            break;
        case WM_KEYUP:
            DispatchEvent(KeyReleasedEvent(KEY_CODES_MAP.at(wParam)));
            break;

        /******************************************************/
        /******************* MOUSE EVENTS ********************/
        // Perform the same action for basic mouse buttons
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            SetCapture(m_hwnd);
            DispatchEvent(MouseButtonPressedEvent(m_mouseButtonsCodesMap.At(uMsg), GetMouseCoordinates(lParam)));
            break;
        case WM_XBUTTONDOWN:
            // We need to get the mouse button code differently here
            SetCapture(m_hwnd);
            DispatchEvent(MouseButtonPressedEvent(m_mouseButtonsCodesMap.At(GET_XBUTTON_WPARAM(wParam)), GetMouseCoordinates(lParam)));
            break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            ReleaseCapture();
            DispatchEvent(MouseButtonReleasedEvent(m_mouseButtonsCodesMap.At(uMsg), GetMouseCoordinates(lParam)));
            break;
        case WM_XBUTTONUP:
            // We need to get the mouse button code differently here
            ReleaseCapture();
            DispatchEvent(MouseButtonReleasedEvent(m_mouseButtonsCodesMap.At(GET_XBUTTON_WPARAM(wParam)), GetMouseCoordinates(lParam)));
            break;
        case WM_MOUSEMOVE:
            DispatchEvent(MouseMovedEvent(GetMouseCoordinates(lParam)));
            break;
        case WM_MOUSEWHEEL:
            DispatchEvent(MouseScrolledEvent(GET_WHEEL_DELTA_WPARAM(wParam), GetMouseCoordinates(lParam)));
            break;
        // Might be usefull in the future
        case WM_LBUTTONDBLCLK:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        /******************************************************/
        /******************************************************/

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }
    
    void WindowsWindow::ProcessHeldDownMessages() const
    {
        if (IsKeyPressed(VK_LBUTTON))
        {
            DispatchEvent(MouseButtonHeldDownEvent(MouseButtonCode::BUTTON_LEFT));
        }

        if (IsKeyPressed(VK_MBUTTON))
        {
            DispatchEvent(MouseButtonHeldDownEvent(MouseButtonCode::BUTTON_MIDDLE));
        }

        if (IsKeyPressed(VK_RBUTTON))
        {
            DispatchEvent(MouseButtonHeldDownEvent(MouseButtonCode::BUTTON_RIGHT));
        }

        if (IsKeyPressed(VK_XBUTTON1))
        {
            DispatchEvent(MouseButtonHeldDownEvent(MouseButtonCode::BUTTON4));
        }

        if (IsKeyPressed(VK_XBUTTON2))
        {
            DispatchEvent(MouseButtonHeldDownEvent(MouseButtonCode::BUTTON5));
        }
    }
} // namespace be::internals

#endif
