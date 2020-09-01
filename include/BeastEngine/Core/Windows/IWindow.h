#pragma once
#include "BeastEngine/Core/Events/Events.h"
#include "BeastEngine/Core/Types.h"

namespace be
{
	class IWindow
    {
    public:
        virtual ~IWindow() = default;

        /**
         * Defines passed handler to be used whenever any type of the KeyboardEvent occurs.
         * 
         * @see KeyboardEvent
         * 
         * @param handler
         */
        virtual void SetKeyboardEventsHandler(KeyboardEventHandler handler) = 0;

        /**
         * Defines passed handler to be used whenever any type of the MouseEvent occurs.
         * 
         * @see MouseEvent
         * 
         * @param handler
         */
        virtual void SetMouseEventsHandler(MouseEventHandler handler) = 0;

        /**
         * Processes messages received from the operating system and converts them into the engine's events.
         * Those events are then being dispatched to the defined handlers.
         * 
         * @see Set*EventsHandler(...) for reference
         */
        virtual void ProcessInput() = 0;
    };
}
