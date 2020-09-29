#pragma once
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Helpers.h"
#include "BeastEngine/Core/Input/InputCodes.h"

namespace be
{
    enum class MouseEventType
    {
        EVENT_MOUSE_MOVED,
        EVENT_MOUSE_SCROLLED,
        EVENT_MOUSE_BUTTON_PRESSED,
        EVENT_MOUSE_BUTTON_HELD_DOWN,
        EVENT_MOUSE_BUTTON_RELEASED,
    };

    enum class WheelScrollDirection
    {
        SCROLL_UP,
        SCROLL_DOWN,
    };

    class MouseEvent
    {
    public:
        constexpr static const IntVec2 DEFAULT_COORDINATES = {0, 0};
        constexpr static const int16 DEFAULT_SCROLL_VALUE = 0;

        BE_IMPLEMENT_CONSTRUCTORS_DEFAULT(MouseEvent)

        /**
         * Initializes MouseEvent.
         * 
         * @param eventType - what type of mouse event should be created
         * @param coordinates - cooridnates of the mouse cursor
         * @param scrollValue - how much the mouse wheel was scrolled
         * @param buttonCode - which button (if any) was pressed
         */
        MouseEvent(
            MouseEventType eventType,
            IntVec2 coordinates = DEFAULT_COORDINATES,
            int16 scrollValues = DEFAULT_SCROLL_VALUE,
            MouseButtonCode buttonCode = MouseButtonCode::INVALID
        )
            : m_eventType(eventType),
              m_coordinates(coordinates),
              m_scrollValue(scrollValues),
              m_buttonCode(buttonCode)
        {
        }
        virtual ~MouseEvent() = default;

        /**
         * Returns type of the mouse event.
         * @see MouseEventType
         * 
         * @return 
         */
        MouseEventType GetType() const noexcept
        {
            return m_eventType;
        }

        /**
         * Returns coordinates of the mouse cursor as they were when the event occurred.
         * 
         * @return
         */
        const IntVec2& GetCoordinates() const noexcept
        {
            return m_coordinates;
        }

        /**
         * Returns number indicating how much has the mouse wheel been scrolled.
         * If no scrolling occurred, this value is equal to 0
         * 
         * @return 
         */
        int16 GetScrollValue() const noexcept
        {
            return m_scrollValue;
        }

        /**
         * Returns code of the button which triggered the event.
         * If no button took place in the event MouseButtonCode::INVALID is returned
         * 
         * @return 
         */
        MouseButtonCode GetButton() const noexcept
        {
            return m_buttonCode;
        }

    private:
        MouseEventType m_eventType;
        IntVec2 m_coordinates = DEFAULT_COORDINATES;
        int16 m_scrollValue = DEFAULT_SCROLL_VALUE;
        MouseButtonCode m_buttonCode;
    };

    class MouseMovedEvent final : public MouseEvent
    {
    public:
        /**
         * Initializes MouseEvent with given coordinates of the cursor on the screen and the MouseEventType::EVENT_MOUSE_MOVED type.
         * Leaves other MouseEvent parameters with default values
         * 
         * @param coordinates
         */
        MouseMovedEvent(IntVec2 coordinates)
            : MouseEvent(MouseEventType::EVENT_MOUSE_MOVED, std::move(coordinates))
        {
        }
    };

    class MouseScrolledEvent final : public MouseEvent
    {
    public:
        /**
         * Initializes MouseEvent with given scroll values containing data about how much has the mouse wheel been scrolled.
         * Sets event tyep to MouseEventType::EVENT_MOUSE_SCROLLED.
         * Initializes event with cursor position
         * 
         * @param scrollValues
         * @param coordinates
         */
        MouseScrolledEvent(int16 scrollValue, IntVec2 coordinates)
            : MouseEvent(MouseEventType::EVENT_MOUSE_SCROLLED, std::move(coordinates), std::move(scrollValue))
        {
        }
    };

    class MouseButtonPressedEvent final : public MouseEvent
    {
    public:
        /**
         * Initializes MouseEvent with pressed button code, cursor coordinates and MouseEventType::EVENT_MOUSE_BUTTON_PRESSED.
         *
         * @param button
         * @param coordinates
         */
        MouseButtonPressedEvent(MouseButtonCode button, IntVec2 coordinates)
            : MouseEvent(MouseEventType::EVENT_MOUSE_BUTTON_PRESSED, coordinates, DEFAULT_SCROLL_VALUE, button)
        {
        }
    };

    class MouseButtonHeldDownEvent final : public MouseEvent
    {
    public:
        /**
         * Initializes MouseEvent with held down button code, cursor coordinates and MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN.
         * 
         * @param button
         * @param coordinates
         */
        MouseButtonHeldDownEvent(MouseButtonCode button)
            : MouseEvent(MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN, DEFAULT_COORDINATES, DEFAULT_SCROLL_VALUE, button)
        {
        }
    };

    class MouseButtonReleasedEvent final : public MouseEvent
    {
    public:
        /**
         * Initializes MouseEvent with released button code, cursor coordinates and MouseEventType::EVENT_MOUSE_BUTTON_RELEASED.
         * 
         * @param button
         * @param coordinates
         */
        MouseButtonReleasedEvent(MouseButtonCode button, IntVec2 coordinates)
            : MouseEvent(MouseEventType::EVENT_MOUSE_BUTTON_RELEASED, coordinates, DEFAULT_SCROLL_VALUE, button)
        {
        }
    };
} // namespace be
