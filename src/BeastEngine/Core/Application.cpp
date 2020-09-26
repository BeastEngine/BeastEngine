#include "BeastEngine/Core/Application.h"

namespace be
{
    internals::Mouse::Mouse(IWindow& window) noexcept
    {
        window.SetMouseEventsHandler(GetEventHandler());
    }

    void internals::Mouse::SetWheelScrollThreshold(uint16 threshold) noexcept
    {
        m_scrollThreshold = threshold;
    }

    void internals::Mouse::SetWheelScrolledListener(MouseWheelScrolledListener listener) noexcept
    {
        m_mouseScrolledListener = listener;
    }

    MouseEventHandler internals::Mouse::GetEventHandler() noexcept
    {
        return [&](const MouseEvent& event) {
            m_coordinates = event.GetCoordinates();

            switch (event.GetType())
            {
            case MouseEventType::EVENT_MOUSE_BUTTON_PRESSED:
                m_buttonsStates[event.GetButton()] = true;
                break;
            case MouseEventType::EVENT_MOUSE_BUTTON_RELEASED:
                m_buttonsStates[event.GetButton()] = false;
                break;
            case MouseEventType::EVENT_MOUSE_SCROLLED:
                ScrollWheel(event.GetScrollValue());
                break;
            default:
                return;
            }
        };
    }

    void internals::Mouse::ScrollWheel(int16 scrollAmount) noexcept
    {
        m_currentScrollValue += scrollAmount;
        if (!m_mouseScrolledListener)
        {
            return;
        }

        while (m_currentScrollValue >= m_scrollThreshold)
        {
            m_currentScrollValue -= m_scrollThreshold;
            m_mouseScrolledListener(WheelScrollDirection::SCROLL_UP);
        }
        while (m_currentScrollValue <= -m_scrollThreshold)
        {
            m_currentScrollValue += m_scrollThreshold;
            m_mouseScrolledListener(WheelScrollDirection::SCROLL_DOWN);
        }
    }

    AApplication::AApplication(EngineConfig engineConfig, const WindowDescriptor& mainWindowDescriptor)
    {
        m_engine = CreateUniquePtr<BeastEngine>(std::move(engineConfig));
        m_window = m_engine->CreateMainWindow(mainWindowDescriptor);
        m_mouse = CreateUniquePtr<internals::Mouse>(*m_window);
    }
} // namespace be
