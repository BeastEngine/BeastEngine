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
            this
        );

        if (m_hwnd == NULL)
        {
            throw std::runtime_error("Could not create window. Reason: " + GetLastError());
        }

        DisplayWindow();
    }

    WindowsWindow::~WindowsWindow()
    {
        if (m_hwnd)
        {
            DestroyWindow(m_hwnd);
        }
        UnregisterClass(WINDOW_CLASS_NAME, m_hInstance);
    }

    void WindowsWindow::ProcessInput()
    {
        MSG msg = {};
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
        case WM_DESTROY:
            DispatchWindowClosedEvent();
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
        }

        return 0;
    }
} // namespace be::internals

#endif
