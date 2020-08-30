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
         * Defines passed handler to be used whenever KeyboardEvent occurs.
         * 
         * @param handler
         */
        virtual void SetEventHandler(KeyboardEventHandler handler) = 0;

        /**
         * Defines passed handler to be used whenever KeyPressedEvent occurs.
         * 
         * @param handler
         */
        virtual void SetEventHandler(KeyPressedEventHandler handler) = 0;

        /**
         * Defines passed handler to be used whenever KeyDownEvent occurs.
         * 
         * @param handler
         */
        virtual void SetEventHandler(KeyDownEventHandler handler) = 0;

        /**
         * Defines passed handler to be used whenever KeyReleasedEvent occurs.
         * 
         * @param handler
         */
        virtual void SetEventHandler(KeyReleasedEventHandler handler) = 0;

        /**
         * Processes messages received from the operating system and converts them into the engine's events.
         * Those events are then being dispatched to the defined handlers.
         * 
         * @see SetEventHandler(...) for reference
         */
        virtual void ProcessInput() = 0;
    };
}
