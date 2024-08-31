#pragma once
#include "Beast/PlatformSetup.hpp"
#include "Beast/Input/Input.hpp"

#include "Beast/Common/Types.hpp"
#include "Beast/Common/Helpers.hpp"

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
    /****************** CLASS DEFINITION ******************/

    /**
     * @brief BeastEngine window.
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

        /**
         * @brief Returns underlying input handler that processes this window's system messages.
         */
        const Input& GetInputHandler() const noexcept;

        /**
         * @brief Returns info if window closing was requested.
         */
        bool ShouldClose() const noexcept;

    private:
        struct Impl;
        be::Unique<Impl> m_impl = nullptr;
    };

    /******************************************************/
    /******************************************************/
} // namespace be
