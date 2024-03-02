#pragma once
#include "Beast/Windows/Window.h"
#include "Beast/Debug.h"

#include "Beast/Common/Types.h"

namespace be
{
    /**
     * @brief Abstract, base class for all windows.
     * It provides its children with basic functionality for setting events handlers, and dispatching those events.
     */
    class AWindow
    {
    public:
        void SetKeyboardEventsHandler(KeyboardEventHandler handler)
        {
            m_keyboardEventsHandler = handler;
        }

        void SetMouseEventsHandler(MouseEventHandler handler)
        {
            m_mouseEventsHandler = handler;
        }

        void SetWindowClosedEventHandler(WindowClosedEventHandler handler)
        {
            m_windowClosedEventHandler = handler;
        }

    protected:
        /**
         * @brief Dispatches provided KeyboardEvent if event handler is defined.
         * Leaves event unhandled otherwise.
         * 
         * @param event to dispatch
         */
        void DispatchEvent(const KeyboardEvent& event) const
        {
            if (m_keyboardEventsHandler)
            {
                m_keyboardEventsHandler(event);
                return;
            }

            BE_DEBUG_LOG_WARNING("No handler specified for KeyboardEvents");
        }

        /**
         * @brief Dispatches provided MouseEvent if event handler is defined.
         * Leaves event unhandled otherwise.
         * 
         * @param event to dispatch
         */
        void DispatchEvent(const MouseEvent& event) const
        {
            if (m_mouseEventsHandler)
            {
                m_mouseEventsHandler(event);
                return;
            }

            BE_DEBUG_LOG_WARNING("No handler specified for MouseEvents");
        }

        /**
         * @brief Dispatches WindowClosedEvent if event handler is defined.
         * Leaves event unhandled otherwise.
         */
        void DispatchWindowClosedEvent() const
        {
            if (m_windowClosedEventHandler)
            {
                m_windowClosedEventHandler();
                return;
            }

            BE_DEBUG_LOG_WARNING("No handler specified for WindowClosed event");
        }

    private:
        KeyboardEventHandler m_keyboardEventsHandler;
        MouseEventHandler m_mouseEventsHandler;
        WindowClosedEventHandler m_windowClosedEventHandler;
    };
} // namespace be
