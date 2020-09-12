#pragma once
#include "IWindow.h"
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Logging.h"

namespace be
{
    class AWindow : public IWindow
    {
    public:
        virtual ~AWindow() = default;
        
        virtual void SetKeyboardEventsHandler(KeyboardEventHandler handler) override
        {
            m_keyboardEventsHandler = handler;
        }
        
        virtual void SetMouseEventsHandler(MouseEventHandler handler) override
        {
            m_mouseEventsHandler = handler;
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
            if (m_keyboardEventsHandler != nullptr)
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
            if (m_mouseEventsHandler != nullptr)
            {
                m_mouseEventsHandler(event);
                return;
            }

            BE_DEBUG_LOG_WARNING("No handler specified for MouseEvents");
        }

    private:
        KeyboardEventHandler m_keyboardEventsHandler;
        MouseEventHandler m_mouseEventsHandler;
    };
} // namespace be
