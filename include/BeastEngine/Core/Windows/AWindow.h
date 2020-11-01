#pragma once
#include "IWindow.h"
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Debugging.h"

namespace be
{
    class AWindow : public IWindow
    {
    public:
        virtual ~AWindow() = default;

        void SetKeyboardEventsHandler(KeyboardEventHandler handler) override
        {
            m_keyboardEventsHandler = handler;
        }

        void SetMouseEventsHandler(MouseEventHandler handler) override
        {
            m_mouseEventsHandler = handler;
        }

        void SetWindowClosedEventHandler(WindowClosedEventHandler handler) override
        {
            m_windowClosedEventHandler = handler;
        }

    protected:
        /**
         * Dispatches provided KeyboardEvent if event handler is defined.
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
         * Dispatches provided MouseEvent if event handler is defined.
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
         * Dispatches WindowClosedEvent if event handler is defined.
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
