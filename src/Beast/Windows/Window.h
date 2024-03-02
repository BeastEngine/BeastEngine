#pragma once
#include "Beast/PlatformSetup.h"
#include "Beast/Input/Input.h"

#include "Beast/Common/Types.h"
#include "Beast/Common/Helpers.h"

namespace be
{
    /******************************************************/
    /******************** WINDOW STYLE ********************/
    enum class WindowStyle
    {
        WINDOW_FULLSCREEN, // Window is taking the whole screen and is the topmost window
        WINDOW_BORDERLESS, // Maximized, normal window taking the whole screen area
        WINDOW_DEFUALT, // Default, normal window with title and sizing buttons
    };
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /******************* WINDOW PARAMS ********************/

    /**
     * @brief Wrapper for platform specific window handle.
     */
    class WindowHandleInstance final
    {
    public:
        /**
         * @brief Initializes WindowHandleInstanceType class with platform specific instance.
         * 
         * @param instance
         */
        WindowHandleInstance(WindowHandleInstanceType instance)
            : m_instance(instance)
        {
        }

        /**
         * @brief Returns platform specific instance.
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

    /**
     * @brief Contains configuration options for windows.
     */
    struct WindowDescriptor
    {
        WindowDescriptor(WindowHandleInstance wHInstance)
            : handleInstance(wHInstance)
        {
        }

        std::string title = "BeastEngine";
        Vec2i dimensions = {800, 600};
        Vec2i position = {0, 0};
        WindowStyle style = {WindowStyle::WINDOW_DEFUALT};
        const WindowHandleInstance handleInstance;
    };
    /******************************************************/
    /******************************************************/

    /******************************************************/
    /**************** INTERFACE DEFINITION ****************/
    /**
     * @brief Basic interface for engine's windows.
     */
    class Window
    {
    public:
        Window(const WindowDescriptor& windowDescriptor);
        Window(const Window&) = delete;
        Window& operator=(const Window&) noexcept = delete;
        Window(Window&&);
        Window& operator=(Window&&) noexcept;
        ~Window();

        /**
         * @brief Saves passed handler to be used whenever any type of the KeyboardEvent occurs.
         * @see KeyboardEvent
         * 
         * @param handler
         */
        void SetKeyboardEventsHandler(KeyboardEventHandler handler);

        /**
         * @brief Saves passed handler to be used whenever any type of the MouseEvent occurs.
         * @see MouseEvent
         * 
         * @param handler
         */
        void SetMouseEventsHandler(MouseEventHandler handler);

        /**
         * @brief Saves passed handler to be used whenever WindowClosed event occurs.
         * 
         * @param handler
         */
        void SetWindowClosedEventHandler(WindowClosedEventHandler handler);

        /**
         * @brief Processes messages received from the operating system and converts them into the engine's events.
         * Those events are then being dispatched to the defined handlers.
         * 
         * @see Set*EventsHandler() for reference
         */
        void ProcessInput();

        /**
         * @brief Returns handle associated with this window.
         */
        WindowHandle GetHandle() const noexcept;

        /**
         * @brief Returns current windows size.
         */
        const Vec2i& GetDimensions() const noexcept;

        const Input& GetInputHandler() const noexcept;

    private:
        struct Impl;
        be::Unique<Impl> m_impl = nullptr;
    };
    /******************************************************/
    /******************************************************/
} // namespace be
