#pragma once
#include <Beast/Input/InputCodes.h>

#include <Beast/Math/Types.h>

namespace be
{
    enum class MouseEventType
    {
        EVENT_MOUSE_MOVED,
        EVENT_MOUSE_SCROLLED,
        EVENT_MOUSE_BUTTON_PRESSED, // Pressed once, in the current frame
        EVENT_MOUSE_BUTTON_HELD_DOWN, // Was pressed and is now held down
        EVENT_MOUSE_BUTTON_RELEASED, // Released in the current frame
    };

    enum class WheelScrollDirection
    {
        SCROLL_UP,
        SCROLL_DOWN,
    };

    /**
     * @brief Stores single mouse event data.
     */
    class MouseEvent final
    {
    public:
        constexpr static const IntVec2 DEFAULT_COORDINATES = {0, 0};
        constexpr static const int16 DEFAULT_SCROLL_VALUE = 0;

        /**
         * @brief Initializes MouseEvent with its information.
         * It accepts multiple values, where every one, except for the event type, can be omitted,
         * depending on the data associated with the event's occurrence.
         * 
         * @param eventType - Type of this event
         * @param mousePosition - Position of the mouse cursor when the event was triggered
         * @param scrollValue - How much the mouse wheel was scrolled (if any)
         * @param buttonCode - Which button (if any) was pressed
         */
        constexpr MouseEvent(
            MouseEventType eventType,
            IntVec2 mousePosition = DEFAULT_COORDINATES,
            int16 scrollValues = DEFAULT_SCROLL_VALUE,
            MouseButtonCode buttonCode = MouseButtonCode::INVALID
        ) noexcept
            : m_eventType(eventType),
              m_coordinates(mousePosition),
              m_scrollValue(scrollValues),
              m_buttonCode(buttonCode)
        {
        }

        /**
         * @brief Creates mouse moved event containing new cursor position.
         * The new event contains default scrollValues and button code equal to MouseButtonCode::INVALID
         * 
         * @param mousePosition - New position of the mouse cursor (position after mouse was moved)
         * @return 
         */
        static constexpr auto Moved(IntVec2 mousePosition) noexcept
        {
            return MouseEvent(MouseEventType::EVENT_MOUSE_MOVED, std::move(mousePosition));
        }

        /**
         * @brief Creates mouse scrolled event containing mouse position and scroll value (how much scrolling occurred).
         * The new event contains button code equal to MouseButtonCode::INVALID
         * 
         * @param scrollValue - How much scrolling occurred
         * @param mousePosition - Position of the mouse cursor when the event occurred
         * @return 
         */
        static constexpr auto Scrolled(int16 scrollValue, IntVec2 mousePosition)
        {
            return MouseEvent(MouseEventType::EVENT_MOUSE_SCROLLED, std::move(mousePosition), std::move(scrollValue));
        }

        /**
         * @brief Creates mouse button pressed event containing button code and mouse position.
         * The new event contains default scroll value.
         * 
         * @param button - Code of the mouse button that triggered the event
         * @param mousePosition - Position of the mouse cursor when the event occurred
         * @return 
         */
        static constexpr auto ButtonPressed(MouseButtonCode button, IntVec2 mousePosition)
        {
            return MouseEvent(MouseEventType::EVENT_MOUSE_BUTTON_PRESSED, std::move(mousePosition), DEFAULT_SCROLL_VALUE, button);
        }

        /**
         * @brief Creates mouse button held down event containing button code.
         * The new event contains default scroll value and default mouse position.
         * 
         * @param button - Code of the mouse button that triggered the event
         * @return 
         */
        static constexpr auto ButtonHeldDown(MouseButtonCode button)
        {
            return MouseEvent(MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN, DEFAULT_COORDINATES, DEFAULT_SCROLL_VALUE, button);
        }

        /**
         * @brief Creates mouse button released event containing button code and mouse position.
         * The new event contains default scroll value.
         * 
         * @param button - Code of the mouse button that triggered the event
         * @param mousePosition - Position of the mouse cursor when the event occurred
         * @return 
         */
        static constexpr auto ButtonReleased(MouseButtonCode button, IntVec2 mousePosition)
        {
            return MouseEvent(MouseEventType::EVENT_MOUSE_BUTTON_RELEASED, std::move(mousePosition), DEFAULT_SCROLL_VALUE, button);
        }

        /**
         * @brief Returns type of the mouse event.
         * 
         * @return 
         */
        constexpr MouseEventType GetType() const noexcept
        {
            return m_eventType;
        }

        /**
         * @brief Returns position of the mouse cursor associated with this event.
         * 
         * @return
         */
        constexpr const IntVec2& GetMousePosition() const noexcept
        {
            return m_coordinates;
        }

        /**
         * @brief Returns number indicating how much the mouse wheel has been scrolled.
         * If no scrolling occurred, this value is equal to 0
         * 
         * @return 
         */
        int16 GetScrollValue() const noexcept
        {
            return m_scrollValue;
        }

        /**
         * @brief Returns code of the button which triggered the event.
         * If event isn't associated with any button, MouseButtonCode::INVALID is returned
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
} // namespace be
