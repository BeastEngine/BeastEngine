#pragma once
#include "IWindow.h"

namespace be
{
    enum class WindowFlags : byte
    {
        WINDOW_FULLSCREEN = bit(1),
        WINDOW_BORDERLESS = bit(2),
        WINDOW_RESIZABLE = bit(3),
    };

    inline WindowFlags operator|(WindowFlags flag1, WindowFlags flag2)
    {
        using T = std::underlying_type_t<WindowFlags>;

        return static_cast<WindowFlags>(static_cast<T>(flag1) | static_cast<T>(flag2));
    }

    struct WindowDescriptor
    {
        std::string title;
        IntVec2 dimensions = {800, 600};
        IntVec2 position = {0.0, 0.0};
        WindowFlags flags = WindowFlags::WINDOW_RESIZABLE | WindowFlags::WINDOW_BORDERLESS;
    };

    class IWindowFactory
    {
    public:
        virtual ~IWindowFactory() = default;

        /**
         * Creates and returns new window.
         * It should be used to create windows inside the engine
         * 
         * @param descriptor - parameters the window will be created with
         * @return pointer to the implementation of the IWindow interface
         */
        virtual UniquePtr<IWindow> Create(const WindowDescriptor& descriptor) = 0;
    };
} // namespace be
