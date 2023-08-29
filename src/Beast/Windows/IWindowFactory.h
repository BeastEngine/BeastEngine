#pragma once
#include <Beast/Windows/IWindow.h>

#include <Beast/Common/Helpers.h>

namespace be
{
    /**
     * @brief Interface for windows factories.
     * It's main purpose is to encapsulate creation of platform specific windows,
     * and to add a possibility of creating custom factories.
     * Eg. If one would like to use GLFW windows instead of Win32 window on Windows platform.
     */
    class IWindowFactory
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(IWindowFactory);
        IWindowFactory() = default;
        virtual ~IWindowFactory() = default;

        /**
         * @brief Creates and returns new instance of the window.
         * It should be used to create windows inside the engine.
         * 
         * @param descriptor - Parameters the window will be created with
         * @return Unique instance of the implementation of the IWindow interface
         */
        virtual Unique<IWindow> Create(const WindowDescriptor& descriptor) = 0;
    };
} // namespace be
