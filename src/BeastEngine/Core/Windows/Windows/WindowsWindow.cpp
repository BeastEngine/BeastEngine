#ifdef BE_PLATFORM_WINDOWS
#include "BeastEngine/Core/Windows/Windows/WindowsWindow.h"
#include "BeastEngine/Core/Assertions.h"    

namespace be::internals
{
    WindowsWindow::WindowsWindow(const WindowDescriptor& windowDescriptor)
        : m_hInstance(windowDescriptor.handleInstance.Get())
    {
        WNDCLASS wc = {0};

        wc.lpfnWndProc = WindowProcSetup;
        wc.hInstance = m_hInstance;
        wc.lpszClassName = WINDOW_CLASS_NAME;
        RegisterClass(&wc);

        m_hwnd = CreateWindowExW(
            0,
            WINDOW_CLASS_NAME,
            ConvertWindowTitle(windowDescriptor.title).c_str(),
            GetWindowFlags(windowDescriptor.flags),
            windowDescriptor.position.x,
            windowDescriptor.position.y,
            windowDescriptor.dimensions.x,
            windowDescriptor.dimensions.y,
            NULL, // Parent window
            NULL, // Menu
            wc.hInstance,
            NULL // Additional application data
        );
        
        if (m_hwnd == NULL)
        {
            throw std::runtime_error("Could not create window");
        }

        ShowWindow(m_hwnd, SW_SHOW);
    }

    WindowsWindow::~WindowsWindow()
    {
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
        int wcharBufferSize = MultiByteToWideChar(CP_UTF8, 0, narrowTitle.c_str(), -1, NULL, 0);
        auto wideTitle = std::wstring(wcharBufferSize, 0);

        const auto result = MultiByteToWideChar(CP_UTF8, 0, narrowTitle.c_str(), -1, &wideTitle[0], wcharBufferSize);
        BE_ASSERT(result);

        return wideTitle;
    }
    
    uint32 WindowsWindow::GetWindowFlags(const std::vector<WindowFlags>& userDefinedFlags) const
    {
        uint32 windowFlags = 0;
        for (const auto& flag : userDefinedFlags)
        {
            windowFlags |= m_windowFlagsMap.At(flag);
        }

        return windowFlags;
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
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(owningWindow));
        
        // Set window proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::WindowProcThunk));
        
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
        case WM_QUIT:
            PostQuitMessage(0);
        case WM_CREATE:
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
        }

        return 0;
    }
} // namespace be::internals

#endif
