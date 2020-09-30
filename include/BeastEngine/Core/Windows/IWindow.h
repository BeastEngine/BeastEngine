#pragma once
#include "BeastEngine/Core/PlatformSetup.h"
#include "BeastEngine/Core/Events/Events.h"
#include "BeastEngine/Core/Types.h"

namespace be
{
    /******************************************************/
    /******************** WINDOW STYLE ********************/
    enum class WindowStyle
    {
        WINDOW_FULLSCREEN, // Window is taking the whole screen and is the topmost windoww
        WINDOW_BORDERLESS, // Maximized normal window taking the whole screen area
        WINDOW_DEFUALT, // Default, normal window with title and sizing buttons
    };
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /******************* WINDOW PARAMS ********************/
    class WindowHandleInstance final
    {
    public:
        /**
         * Initializes WindowHandleInstance class with platform specific instance.
         * 
         * @param instance
         */
        WindowHandleInstance(WindowHandleInstanceType instance)
            : m_instance(instance)
        {
        }
        ~WindowHandleInstance() = default;

        /**
         * Returns platform specific instance.
         * 
         * @return 
         */
        WindowHandleInstanceType Get() const
        {
            return m_instance;
        }

    private:
        WindowHandleInstanceType m_instance;
    };

    struct WindowDescriptor
    {
        WindowDescriptor(WindowHandleInstance wHInstance)
            : handleInstance(wHInstance)
        {
        }

        std::string title = "BeastEngine";
        IntVec2 dimensions = {800, 600};
        IntVec2 position = {0.0, 0.0};
        WindowStyle style = {WindowStyle::WINDOW_DEFUALT};
        WindowHandleInstance handleInstance;
    };
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /**************** INTERFACE DEFINITION ****************/
    class IWindow
    {
    public:
        virtual ~IWindow() = default;

        /**
         * Saves passed handler to be used whenever any type of the KeyboardEvent occurs.
         * 
         * @see KeyboardEvent
         * 
         * @param handler
         */
        virtual void SetKeyboardEventsHandler(KeyboardEventHandler handler) = 0;

        /**
         * Saves passed handler to be used whenever any type of the MouseEvent occurs.
         * 
         * @see MouseEvent
         * 
         * @param handler
         */
        virtual void SetMouseEventsHandler(MouseEventHandler handler) = 0;

        /**
         * Saves passed handler to be used whenever WindowClosed event occurs.
         * 
         * @param handler
         */
        virtual void SetWindowClosedEventHandler(WindowClosedEventHandler handler) = 0;

        /**
         * Processes messages received from the operating system and converts them into the engine's events.
         * Those events are then being dispatched to the defined handlers.
         * 
         * @see Set*EventsHandler(...) for reference
         */
        virtual void ProcessInput() = 0;
    };
    /******************************************************/
    /******************************************************/
} // namespace be
