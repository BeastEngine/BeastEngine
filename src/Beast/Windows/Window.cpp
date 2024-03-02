#include "Beast/Windows/Window.h"
#ifdef BE_PLATFORM_WINDOWS
    #include "Beast/Windows/Win32/Win32Window.h"
    #include "Beast/Common/UUIDGenerator.h"

    #include <string>
#else
BE_THROW("Platform not supported! Can't create a window.");
#endif

namespace be
{
#ifdef BE_PLATFORM_WINDOWS
    using WindowT = internals::Win32Window;
#endif

    struct Window::Impl
    {
#ifdef BE_PLATFORM_WINDOWS
        Window::Impl(const WindowDescriptor& windowDescriptor)
            : win(windowDescriptor, std::to_wstring(GenerateUUID4().Raw()) + L"_BeastEngineWindowClassName")
        {
        }
#endif
        WindowT win;
    };

    Window::Window(const WindowDescriptor& windowDescriptor)
        : m_impl(MakeUnique<Impl>(windowDescriptor))
    {
    }

    Window::Window(Window&&) = default;
    Window& Window::operator=(Window&&) noexcept = default;

    Window::~Window() = default;

    void Window::SetKeyboardEventsHandler(KeyboardEventHandler handler)
    {
        m_impl->win.SetKeyboardEventsHandler(handler);
    }

    void Window::SetMouseEventsHandler(MouseEventHandler handler)
    {
        m_impl->win.SetMouseEventsHandler(handler);
    }

    void Window::SetWindowClosedEventHandler(WindowClosedEventHandler handler)
    {
        m_impl->win.SetWindowClosedEventHandler(handler);
    }

    void Window::ProcessInput()
    {
        m_impl->win.ProcessInput();
    }

    WindowHandle Window::GetHandle() const noexcept
    {
        return m_impl->win.GetHandle();
    }

    const Vec2i& Window::GetDimensions() const noexcept
    {
        return m_impl->win.GetDimensions();
    }

    const Input& Window::GetInputHandler() const noexcept
    {
        return m_impl->win.GetInputHandler();
    }
} // namespace be
