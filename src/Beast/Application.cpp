#include "Beast/Application.h"

namespace be
{
    /******************************************************************************/
    /********** CREATED FOR TESTING PURPOSES. THIS IS SUBJECT TO CHANGE **********/
    Mouse::Mouse(IWindow& window) noexcept
    {
        window.SetMouseEventsHandler(GetEventHandler());
    }

    MouseEventHandler Mouse::GetEventHandler() noexcept
    {
        return [&](const MouseEvent& event) {
            if (event.GetType() != MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN)
            {
                m_coordinates = event.GetMousePosition();
            }

            auto& buttonState = m_buttonsStates[event.GetButton()];
            switch (event.GetType())
            {
            case MouseEventType::EVENT_MOUSE_BUTTON_PRESSED:
                buttonState.isPressed = true;
                break;
            case MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN:
                if (buttonState.isPressed == false)
                {
                    return;
                }

                buttonState.isPressed = false;
                buttonState.isHeldDown = true;
                break;
            case MouseEventType::EVENT_MOUSE_BUTTON_RELEASED:
                buttonState.isPressed = false;
                buttonState.isHeldDown = false;
                break;
            case MouseEventType::EVENT_MOUSE_SCROLLED:
                ScrollWheel(event.GetScrollValue());
                break;
            default:
                return;
            }
        };
    }

    void Mouse::ScrollWheel(int16 scrollAmount) noexcept
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

    Keyboard::Keyboard(IWindow& window) noexcept
    {
        window.SetKeyboardEventsHandler(GetEventHandler());
    }

    KeyboardEventHandler be::Keyboard::GetEventHandler() noexcept
    {
        return [&](const KeyboardEvent& event) {
            auto& buttonState = m_buttonsStates[event.GetKey()];
            switch (event.GetType())
            {
            case KeyboardEventType::EVENT_KEY_PRESSED:
                buttonState.isPressed = true;
                break;
            case KeyboardEventType::EVENT_KEY_HELD_DOWN:
                if (buttonState.isPressed == false)
                {
                    return;
                }

                buttonState.isPressed = false;
                buttonState.isHeldDown = true;
                break;
            case KeyboardEventType::EVENT_KEY_RELEASED:
                buttonState.isPressed = false;
                buttonState.isHeldDown = false;
                break;
            default:
                return;
            }
        };
    }
    /******************************************************************************/
    /******************************************************************************/

    AApplication::AApplication(EngineConfig engineConfig, const WindowDescriptor& mainWindowDescriptor)
    {
        m_engine = MakeUnique<BeastEngine>(std::move(engineConfig));
        m_window = m_engine->CreateNewWindow(mainWindowDescriptor);
        m_mouse = MakeUnique<Mouse>(*m_window);
        m_keyboard = MakeUnique<Keyboard>(*m_window);

        m_window->SetWindowClosedEventHandler(OnWindowClosed());
    }

    void AApplication::Start()
    {
        try
        {
            m_engine->PrintInfo();
            Run();
        }
        catch (const std::exception& error)
        {
            BE_DISPLAY_ERROR(error);
        }
    }
} // namespace be
