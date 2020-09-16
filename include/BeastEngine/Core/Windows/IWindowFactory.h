#pragma once
#include "IWindow.h"

namespace be
{
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
