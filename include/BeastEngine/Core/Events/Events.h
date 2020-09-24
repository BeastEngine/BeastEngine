#pragma once
#include "KeyboardEvents.h"
#include "MouseEvents.h"

namespace be
{
    /********************EVENT HANDLERS DEFINITIONS*************************/
    // Keyboard
    using KeyboardEventHandler = std::function<void(const KeyboardEvent&)>;

    // Mouse
    using MouseEventHandler = std::function<void(const MouseEvent&)>;

    // Window
    using WindowClosedEventHandler = std::function<void()>;
    /***********************************************************************/
} // namespace be
